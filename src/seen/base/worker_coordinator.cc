// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/29.

#include "seen/base/worker_coordinator.h"
#include "seen/base/logger.h"

namespace seen {

WorkerCoordinator::WorkerCoordinator(CFWorker::Ptr host, const std::vector<CFWorker::Ptr>& cooperators)
    : host_(std::move(host)), host_latch_(std::make_shared<CFAutoResetWaitableEvent>()) {
  SEEN_ASSERT(host_->IsCurrent() && !cooperators.empty());
  for (auto&& cooperator : cooperators) {
    auto latch = std::make_shared<CFAutoResetWaitableEvent>();
    latch_map_[cooperator] = latch;
    cooperator->DispatchAsync([this, latch]() {
      host_latch_->Signal();
      latch->Wait();
      while (current_task_) {
        current_task_();
        current_task_ = nullptr;
        latch->Wait();
      }
      host_latch_->Signal();
    });
    host_latch_->Wait();
  }
}

WorkerCoordinator::~WorkerCoordinator() {
  SEEN_ASSERT(!current_task_);
  for (auto&& [cooperator, latch] : latch_map_) {
    latch->Signal();
    host_latch_->Wait();
  }
}

void WorkerCoordinator::Dispatch(const CFWorker::Ptr& worker, CFClosure task) {
  SEEN_ASSERT(!current_task_ && worker && task);
  if (host_->IsCurrent()) {
    task();
    return;
  }
  current_task_ = std::move(task);
  auto latch = latch_map_[worker];
  SEEN_ASSERT(latch);
  latch->Signal();
  host_latch_->Wait();
  SEEN_ASSERT(!current_task_);
}

}  // namespace seen

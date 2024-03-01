// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/base/worker_coordinator.h"
#include "seen/base/logger.h"

namespace seen {

WorkerCoordinator::WorkerCoordinator(Worker::Ptr host, const std::vector<Worker::Ptr>& cooperators)
    : host_(std::move(host)), host_latch_(std::make_shared<AutoResetWaitableEvent>()) {
  SEEN_ASSERT(host_->IsCurrent() && !cooperators.empty());
  for (auto&& cooperator : cooperators) {
    auto latch = std::make_shared<AutoResetWaitableEvent>();
    latch_map_[cooperator] = latch;
    cooperator->DispatchAsync([this, latch]() {
      host_latch_->Signal();
      latch->Wait();
      while (current_task_) {
        current_task_();
        current_task_ = nullptr;
        host_latch_->Signal();
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

void WorkerCoordinator::Dispatch(const Worker::Ptr& worker, Closure task) {
  SEEN_ASSERT(!current_task_ && worker && task);
  if (worker->IsCurrent()) {
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

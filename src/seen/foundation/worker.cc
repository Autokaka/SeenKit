// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#include "seen/foundation/worker.h"

namespace seen {

CFWorker::CFWorker(std::unique_ptr<CFWorkerTrait>&& impl) : impl_(std::move(impl)) {
  impl_->Start();
}

CFWorker::~CFWorker() {
  impl_->Stop();
}

bool CFWorker::IsHost() const {
  return impl_->IsHost();
}

void CFWorker::DispatchAsync(const Closure& macro_task) {
  return DispatchAsync(macro_task, TimePoint::Now());
}

void CFWorker::DispatchAsync(const Closure& macro_task, const TimeDelta& time_delta) {
  return DispatchAsync(macro_task, TimePoint::Now() + time_delta);
}

void CFWorker::DispatchAsync(const Closure& macro_task, const TimePoint& time_point) {
  std::scoped_lock lock(tasks_mutex_);
  tasks_.push({.task = macro_task, .target_time = time_point});
  impl_->SetWakeup(time_point, RunExpiredTasksNow, this);
}

void CFWorker::RunExpiredTasksNow(void* worker) {
  static_cast<CFWorker*>(worker)->RunExpiredTasksNow();
}

void CFWorker::RunExpiredTasksNow() {
  std::vector<Closure> macro_tasks;
  {
    std::scoped_lock lock(tasks_mutex_);
    auto now = TimePoint::Now();
    while (!tasks_.empty()) {
      auto nearest_task = tasks_.top();
      if (nearest_task.target_time > now) {
        break;
      }
      tasks_.pop();
      macro_tasks.emplace_back(nearest_task.task);
    }
  }

  for (const auto& task : macro_tasks) {
    task();
  }
}

}  // namespace seen

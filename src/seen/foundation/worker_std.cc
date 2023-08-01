// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/29.

#include "seen/foundation/worker_std.h"
#include "seen/foundation/waitable_event.h"

namespace seen {

void CFWorkerStd::Start() {
  std::scoped_lock lock(mutex_);
  running_ = true;
  thread_ = std::make_unique<std::thread>([this]() { WaitForNextWakeup(); });
}

void CFWorkerStd::Stop() {
  struct StopContext {
    CFAutoResetWaitableEvent latch;
    CFWorkerStd* self;
  } context{.self = this};
  auto stop = [](void* user_data) {
    auto* context = static_cast<StopContext*>(user_data);
    std::scoped_lock lock(context->self->mutex_);
    context->self->running_ = false;
    context->latch.Signal();
  };
  SetWakeup(TimePoint::Now(), stop, &context);
  context.latch.Wait();
  thread_->detach();
}

bool CFWorkerStd::IsHost() const {
  return std::this_thread::get_id() == thread_->get_id();
}

void CFWorkerStd::SetWakeup(const TimePoint& time_point, void (*on_wake_up)(void*), void* user_data) {
  std::scoped_lock lock(mutex_);
  if (!running_) {
    return;
  }
  WakeupTask task{.task = on_wake_up, .user_data = user_data, .target_time = time_point};
  auto no_tasks_before = wakeup_tasks_.empty();
  const auto& nearest_task = no_tasks_before ? task : wakeup_tasks_.top();
  wakeup_tasks_.push(task);
  if (no_tasks_before || time_point < nearest_task.target_time) {
    signaled_ = true;
    cv_.notify_one();
  }
}

void CFWorkerStd::WaitForNextWakeup() {
  {
    std::unique_lock lock(mutex_);
    auto nearest_time = wakeup_tasks_.empty() ? TimePoint::Max() : wakeup_tasks_.top().target_time;
    auto stop_waiting = [this]() { return signaled_; };
    auto timeout = cv_.wait_until(lock, nearest_time.ToEpochTime(), std::move(stop_waiting));
    signaled_ = false;
    if (timeout) {
      ConsumeWakeupTasksNoLock();
    }
    if (!running_) {
      return;
    }
  }
  WaitForNextWakeup();
}

void CFWorkerStd::ConsumeWakeupTasksNoLock() {
  while (!wakeup_tasks_.empty()) {
    auto task = wakeup_tasks_.top();
    auto timeout = task.target_time <= TimePoint::Now();
    if (timeout) {
      task.task(task.user_data);
    }
    if (timeout || !running_) {
      wakeup_tasks_.pop();
    }
  }
}

std::shared_ptr<CFWorker> CreateWorker(const std::string& name) {
  auto worker_impl = std::make_unique<CFWorkerStd>();
  auto worker = std::make_shared<CFWorker>(std::move(worker_impl));
  return worker;
}

}  // namespace seen

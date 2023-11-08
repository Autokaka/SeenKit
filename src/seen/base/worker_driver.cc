// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/14.

#include <pthread.h>
#include <list>
#include <mutex>

#include "seen/base/logger.h"
#include "seen/base/worker_driver.h"
#include "seen/pal/pal.h"

namespace seen {

#pragma mark - CFWorkerDriverImpl::Context

TimePoint CFWorkerDriverImpl::Context::NextWakeupTime() {
  std::scoped_lock lock(mutex);
  return wakeup_tasks.empty() ? TimePoint::Max() : wakeup_tasks.top().target_time;
}

void CFWorkerDriverImpl::Context::RunExpiredTasks() {
  std::list<CFClosure> expired_tasks;
  {
    std::scoped_lock lock(mutex);
    while (!wakeup_tasks.empty()) {
      const auto& task = wakeup_tasks.top();
      auto is_expired = task.target_time <= TimePoint::Now();
      if (is_expired) {
        expired_tasks.emplace_back(task.callback);
        wakeup_tasks.pop();
      } else {
        break;
      }
    }
  }
  for (const auto& task : expired_tasks) {
    task();
  }
}

#pragma mark - CFWorkerDriverImpl

static constexpr int kPthreadNameMaxLength = 15;

CFWorkerDriverImpl::CFWorkerDriverImpl(const char* name) : name_(name), context_(std::make_shared<Context>()) {}

CFWorkerDriverImpl::~CFWorkerDriverImpl() = default;

void CFWorkerDriverImpl::Start(CFClosure on_started) {
  std::scoped_lock lock(mutex_);
  SEEN_ASSERT(!thread_);
  thread_ = std::make_unique<std::thread>([name = name_, context = context_, on_started = std::move(on_started)]() {
    pthread_setname_np(name.substr(0, kPthreadNameMaxLength).c_str());
    on_started();
    while (!context->is_stopped) {
      if (context->latch.WaitUntil(context->NextWakeupTime())) {
        context->RunExpiredTasks();
      }
    }
    // NOTE: There won't be any `SetWakeup` calls in this thread after `Stop`.
    std::scoped_lock lock(context->mutex);
    while (!context->wakeup_tasks.empty()) {
      context->wakeup_tasks.pop();
    }
  });
  thread_->detach();
}

void CFWorkerDriverImpl::Stop() {
  std::scoped_lock lock(mutex_);
  SEEN_ASSERT(thread_);
  SetWakeup(TimePoint::Now(), [context = context_]() { context->is_stopped = true; });
  thread_.reset();
  context_ = std::make_shared<Context>();
}

bool CFWorkerDriverImpl::IsCurrent() {
  std::scoped_lock lock(mutex_);
  return std::this_thread::get_id() == thread_->get_id();
}

void CFWorkerDriverImpl::SetWakeup(const TimePoint& time_point, CFClosure task) {
  std::scoped_lock context_lock(mutex_);
  std::scoped_lock wakeup_tasks_lock(context_->mutex);
  WakeupTask wakeup_task{.callback = task, .target_time = time_point};
  auto no_tasks_before = context_->wakeup_tasks.empty();
  const auto& nearest_task = no_tasks_before ? wakeup_task : context_->wakeup_tasks.top();
  context_->wakeup_tasks.push(wakeup_task);
  if (no_tasks_before || time_point < nearest_task.target_time) {
    context_->latch.Signal();
  }
}

#pragma mark - CFPlatformWorkerDriver

CFPlatformWorkerDriver::CFPlatformWorkerDriver() = default;

CFPlatformWorkerDriver::~CFPlatformWorkerDriver() = default;

void CFPlatformWorkerDriver::Start(CFClosure on_started) {
  pal::platform_worker_driver_dispatch_async(TimePoint::Now(), std::move(on_started));
}

void CFPlatformWorkerDriver::Stop() {}

[[nodiscard]] bool CFPlatformWorkerDriver::IsCurrent() {
  return pal::worker_driver_is_platform_driver();
}

void CFPlatformWorkerDriver::SetWakeup(const TimePoint& time_point, CFClosure task) {
  pal::platform_worker_driver_dispatch_async(time_point, std::move(task));
}

}  // namespace seen
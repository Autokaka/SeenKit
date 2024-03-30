// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/base/worker.h"
#include "seen/base/logger.h"
#include "seen/pal/pal.h"

namespace seen {

Worker::Ptr Worker::Create(const char* name) {
  return std::make_shared<WorkerImpl>(name);
}

Worker::Ptr Worker::Platform() {
  static auto worker = std::make_shared<PlatformWorker>();
  return worker;
}

void Worker::DispatchAsync(Closure macro_task) {
  return DispatchAsync(std::move(macro_task), TimePoint::Now());
}

void Worker::DispatchAsync(Closure macro_task, const TimeDelta& time_delta) {
  return DispatchAsync(std::move(macro_task), TimePoint::Now() + time_delta);
}

void Worker::DispatchAsync(Closure macro_task, const TimePoint& time_point) {
  DispatchAsync(std::move(macro_task), time_point);
}

WorkerImpl::WorkerImpl(const char* name)
    : name_(name),
      io_context_(std::make_shared<asio::io_context>()),
      work_guard_(asio::make_work_guard(*io_context_)),
      thread_([this]() { io_context_->run(); }) {}

WorkerImpl::~WorkerImpl() {
  asio::post(*io_context_, [this]() {
    for (auto& timer : timers_) {
      timer->cancel();
    }
    SEEN_ASSERT(timers_.empty());
    io_context_->stop();
  });
  thread_.join();
}

bool WorkerImpl::IsCurrent() const {
  return io_context_->get_executor().running_in_this_thread();
}

const char* WorkerImpl::GetName() const {
  return name_.c_str();
}

void WorkerImpl::DispatchAsync(Closure macro_task, const TimePoint& time_point) {
  auto timer = std::make_shared<asio::steady_timer>(*io_context_);
  timer->expires_at(time_point.ToEpochTime());
  timer->async_wait([this, task = std::move(macro_task), timer](const asio::error_code& error) {
    if (!error) {
      task();
    }
    timers_.remove(timer);
  });
  timers_.emplace_back(timer);
}

bool PlatformWorker::IsCurrent() const {
  return pal::platform_worker_is_current();
}

const char* PlatformWorker::GetName() const {
  return "Seen.Platform";
}

void PlatformWorker::DispatchAsync(Closure macro_task, const TimePoint& time_point) {
  pal::platform_worker_dispatch_async(time_point, std::move(macro_task));
}

}  // namespace seen

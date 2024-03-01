// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/base/worker.h"
#include "seen/base/logger.h"
#include "seen/base/worker_driver.h"

namespace seen {

thread_local Worker::WeakPtr tls_worker;
static const char* const kPlatformWorkerName = "Seen.Host";

Worker::Ptr Worker::Create(const char* name) {
  SEEN_DEBUG("Create worker: {}.", name);
  auto worker_driver = std::make_unique<WorkerDriverImpl>(name);
  auto worker = std::make_shared<Worker>(name, std::move(worker_driver));
  Worker::WeakPtr weak_worker = worker;
  worker->driver_->Start([weak_worker]() { tls_worker = weak_worker; });
  return worker;
}

Worker::Ptr Worker::GetCurrent() {
  return tls_worker.lock();
}

Worker::Worker(const char* name, std::unique_ptr<WorkerDriver> driver) : name_(name), driver_(std::move(driver)) {}

Worker::~Worker() {
  SEEN_DEBUG("Release worker: {}.", name_);
  driver_->Stop();
}

bool Worker::IsCurrent() const {
  return driver_->IsCurrent();
}

std::string Worker::GetName() const {
  return name_;
}

void Worker::DispatchAsync(Closure macro_task) {
  return DispatchAsync(std::move(macro_task), TimePoint::Now());
}

void Worker::DispatchAsync(Closure macro_task, const TimeDelta& time_delta) {
  return DispatchAsync(std::move(macro_task), TimePoint::Now() + time_delta);
}

void Worker::DispatchAsync(Closure macro_task, const TimePoint& time_point) {
  driver_->SetWakeup(time_point, std::move(macro_task));
}

Worker::Ptr GetPlatformWorker() {
  static auto platform_driver = std::make_unique<PlatformWorkerDriver>();
  static auto platform_worker = std::make_shared<Worker>(kPlatformWorkerName, std::move(platform_driver));
  return platform_worker;
}

}  // namespace seen

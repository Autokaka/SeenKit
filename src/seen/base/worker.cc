// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#include "seen/base/worker.h"
#include "seen/base/worker_driver.h"

namespace seen {

thread_local CFWorker::WeakPtr tls_worker;

CFWorker::Ptr CFWorker::Create(const char* name) {
  auto worker_driver = std::make_unique<CFWorkerDriverImpl>(name);
  auto worker = std::make_shared<CFWorker>(std::move(worker_driver));
  CFWorker::WeakPtr weak_worker = worker;
  worker->driver_->Start([weak_worker]() { tls_worker = weak_worker; });
  return worker;
}

CFWorker::Ptr CFWorker::GetCurrent() {
  return tls_worker.lock();
}

CFWorker::CFWorker(std::unique_ptr<CFWorkerDriver> driver) : driver_(std::move(driver)) {}

CFWorker::~CFWorker() {
  driver_->Stop();
}

bool CFWorker::IsCurrent() const {
  return driver_->IsCurrent();
}

void CFWorker::DispatchAsync(CFClosure macro_task) {
  return DispatchAsync(std::move(macro_task), TimePoint::Now());
}

void CFWorker::DispatchAsync(CFClosure macro_task, const TimeDelta& time_delta) {
  return DispatchAsync(std::move(macro_task), TimePoint::Now() + time_delta);
}

void CFWorker::DispatchAsync(CFClosure macro_task, const TimePoint& time_point) {
  driver_->SetWakeup(time_point, std::move(macro_task));
}

CFWorker::Ptr GetPlatformWorker() {
  static auto platform_driver = std::make_unique<CFPlatformWorkerDriver>();
  static auto platform_worker = std::make_shared<CFWorker>(std::move(platform_driver));
  return platform_worker;
}

}  // namespace seen

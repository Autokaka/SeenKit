// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#include "seen/foundation/worker.h"
#include "seen/foundation/worker_driver.h"

namespace seen {

thread_local CFWorker::Weak thread_local_worker;

CFWorker::Ptr CFWorker::GetCurrent() {
  return thread_local_worker.lock();
}

CFWorker::CFWorker(std::unique_ptr<CFWorkerDriver> driver) : driver_(std::move(driver)) {
  driver_->Start([](const CFWorker::Ptr& worker) { thread_local_worker = worker; }, shared_from_this());
}

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

CFWorker::Ptr CreateWorker(const char* name) {
  return std::make_shared<CFWorker>(std::make_unique<CFWorkerDriverImpl>(name));
}

}  // namespace seen

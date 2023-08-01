// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/30.

#if SEEN_BUILD_DARWIN

#import <Foundation/NSThread.h>

#include "seen/foundation/darwin/worker.h"

namespace seen {

std::shared_ptr<CFWorker> GetPlatformWorker() {
  static auto worker_impl = std::make_unique<CFWorkerDarwin>();
  static auto worker = std::make_shared<CFWorker>(std::move(worker_impl));
  return worker;
}

CFWorkerDarwin::CFWorkerDarwin() : main_queue_(dispatch_get_main_queue()) {}

void CFWorkerDarwin::Start() {}

void CFWorkerDarwin::Stop() {}

bool CFWorkerDarwin::IsHost() const {
  return NSThread.isMainThread == YES;
}

void CFWorkerDarwin::SetWakeup(const TimePoint& time_point, void (*on_wake_up)(void*), void* user_data) {
  auto delay = time_point - TimePoint::Now();
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delay.ToNanoseconds()), main_queue_, ^{
    on_wake_up(user_data);
  });
}

}  // namespace seen

#endif  // SEEN_BUILD_DARWIN

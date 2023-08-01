// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/17.

#include <chrono>

#include "seen/foundation/time_point.h"
#include "seen/foundation/waitable_event.h"

namespace seen {

CFAutoResetWaitableEvent::CFAutoResetWaitableEvent() : signaled_{false} {}

void CFAutoResetWaitableEvent::Wait() {
  std::unique_lock lock(mutex_);
  cv_.wait(lock, [this]() { return signaled_; });
  signaled_ = false;
}

void CFAutoResetWaitableEvent::Signal() {
  std::scoped_lock lock(mutex_);
  signaled_ = true;
  cv_.notify_one();
}

}  // namespace seen

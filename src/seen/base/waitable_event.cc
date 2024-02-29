// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/17.

#include "seen/base/waitable_event.h"

namespace seen {

CFAutoResetWaitableEvent::CFAutoResetWaitableEvent() : stop_waiting_(false) {}

void CFAutoResetWaitableEvent::Wait() {
  std::unique_lock lock(cv_mutex_);
  cv_.wait(lock, [this]() { return stop_waiting_; });
  stop_waiting_ = false;
}

bool CFAutoResetWaitableEvent::WaitUntil(const TimePoint& time_point) {
  std::unique_lock lock(cv_mutex_);
  auto is_timeout = cv_.wait_until(lock, time_point.ToEpochTime(), [this]() {  //
    return stop_waiting_;
  });
  stop_waiting_ = false;
  return is_timeout;
}

void CFAutoResetWaitableEvent::Signal() {
  std::scoped_lock lock(cv_mutex_);
  stop_waiting_ = true;
  cv_.notify_one();
}

}  // namespace seen

// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/17.

#include "seen/base/waitable_event.h"

namespace seen {

AutoResetWaitableEvent::AutoResetWaitableEvent() : stop_waiting_(false) {}

void AutoResetWaitableEvent::Wait() {
  std::unique_lock lock(cv_mutex_);
  cv_.wait(lock, [this]() { return stop_waiting_; });
  stop_waiting_ = false;
}

bool AutoResetWaitableEvent::WaitUntil(const TimePoint& time_point) {
  std::unique_lock lock(cv_mutex_);
  auto is_timeout = cv_.wait_until(lock, time_point.ToEpochTime(), [this]() {  //
    return stop_waiting_;
  });
  stop_waiting_ = false;
  return is_timeout;
}

void AutoResetWaitableEvent::Signal() {
  std::scoped_lock lock(cv_mutex_);
  stop_waiting_ = true;
  cv_.notify_one();
}

}  // namespace seen

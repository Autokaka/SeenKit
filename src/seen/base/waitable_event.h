// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/17.

#pragma once

#include <condition_variable>
#include <mutex>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"

namespace seen {

class CFAutoResetWaitableEvent final {
 public:
  CFAutoResetWaitableEvent();

  void Signal();
  void Wait();
  bool WaitUntil(const TimePoint& time_point);

 private:
  std::condition_variable cv_;
  std::mutex cv_mutex_;
  bool stop_waiting_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFAutoResetWaitableEvent);
};

}  // namespace seen

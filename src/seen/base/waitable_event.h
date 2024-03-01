// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <condition_variable>
#include <mutex>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"

namespace seen {

class AutoResetWaitableEvent final {
 public:
  AutoResetWaitableEvent();

  void Signal();
  void Wait();
  bool WaitUntil(const TimePoint& time_point);

 private:
  std::condition_variable cv_;
  std::mutex cv_mutex_;
  bool stop_waiting_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(AutoResetWaitableEvent);
};

}  // namespace seen

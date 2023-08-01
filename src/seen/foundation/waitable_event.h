// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/17.

#pragma once

#include <mutex>

#include "seen/foundation/class_constraints.h"
#include "seen/foundation/time_delta.h"

namespace seen {

class CFAutoResetWaitableEvent final {
 public:
  CFAutoResetWaitableEvent();

  void Signal();
  void Wait();

 private:
  std::condition_variable cv_;
  std::mutex mutex_;
  bool signaled_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFAutoResetWaitableEvent);
};

}  // namespace seen

/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.
 */

#pragma once

#include <mutex>

#include "foundation/class_constraints.h"

namespace seen {

class CFLatch final {
 public:
  CFLatch();

  void Signal();
  void Wait();

 private:
  std::condition_variable cv_;
  std::mutex mutex_;
  bool signaled_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFLatch);
};
}  // namespace seen

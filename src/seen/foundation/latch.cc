/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.
 */

#include "seen/foundation/latch.h"

namespace seen {

CFLatch::CFLatch() : signaled_{false} {}

void CFLatch::Wait() {
  std::unique_lock lock(mutex_);
  while (!signaled_) {
    cv_.wait(lock);
  }
  signaled_ = false;
}

void CFLatch::Signal() {
  std::scoped_lock lock(mutex_);
  signaled_ = true;
  cv_.notify_one();
}

}  // namespace seen

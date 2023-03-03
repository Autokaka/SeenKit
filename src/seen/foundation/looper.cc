/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.
 */

#include <mutex>

#include "seen/foundation/latch.h"
#include "seen/foundation/logger.h"
#include "seen/foundation/looper.h"

namespace seen {

thread_local CFLooperPtr tls_looper;

CFLooper::CFLooper() : is_running_(true), thread_(&CFLooper::Start, this) {}

CFLooper::~CFLooper() {
  Stop();
  thread_.join();
}

bool CFLooper::IsCurrentThreadLooper() const {
  return std::this_thread::get_id() == thread_.get_id();
}

void CFLooper::DispatchAsync(const Closure& macro_task) {
  if (IsCurrentThreadLooper()) {
    macro_task();
    return;
  }

  {
    std::scoped_lock lock(mutex_);
    macro_tasks_.emplace_back(macro_task);
  }
  cv_.notify_one();
}

void CFLooper::DispatchMicro(const Closure& micro_task) {
  if (IsCurrentThreadLooper()) {
    micro_task();
    return;
  }

  {
    std::scoped_lock lock(mutex_);
    micro_tasks_.emplace_back(micro_task);
  }
  cv_.notify_one();
}

void CFLooper::Start() {
  MakeThreadLocalLooper();

  std::vector<Closure> macro_tasks;

  while (is_running_) {
    {
      std::unique_lock lock(mutex_);
      cv_.wait(lock, [this] { return !macro_tasks_.empty() || !micro_tasks_.empty(); });
      std::swap(macro_tasks, macro_tasks_);
    }

    CFLatch latch;
    ConsumeTasks([this, &macro_tasks, &latch]() {
      ConsumeMicroTasks();

      for (const Closure& macro_task : macro_tasks) {
        ConsumeMicroTasks();
        macro_task();
      }
      latch.Signal();
    });
    latch.Wait();
    macro_tasks.clear();
  }
}

void CFLooper::Stop() {
  CFLatch latch;
  DispatchAsync([this, &latch]() {
    is_running_ = false;
    latch.Signal();
  });
  latch.Wait();
}

void CFLooper::MakeThreadLocalLooper() {
  tls_looper = shared_from_this();
}

void CFLooper::ConsumeTasks(const Closure& consumer) {
  consumer();
}

void CFLooper::ConsumeMicroTasks() {
  std::vector<Closure> micro_tasks;
  {
    std::scoped_lock lock(mutex_);
    std::swap(micro_tasks, micro_tasks_);
  }
  for (const auto& micro_task : micro_tasks) {
    micro_task();
  }
  micro_tasks.clear();
}

CFLooperPtr CFLooperGetCurrent() {
  auto platform_looper = CFGetPlatformLooper();
  SEEN_ASSERT(platform_looper != nullptr);
  if (platform_looper->IsCurrentThreadLooper()) {
    return platform_looper;
  }
  SEEN_ASSERT(tls_looper != nullptr);
  return tls_looper;
}

CFLooperPtr CFCreateLooper() {
  return std::make_shared<CFLooper>();
}

}  // namespace seen

/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.
 */

#include "seen/foundation/looper.h"

namespace seen {

CFLooper::CFLooper() : is_running_(true), thread_(&CFLooper::Run, this) {}

CFLooper::~CFLooper() {
  DispatchAsync([this]() { is_running_ = false; }).Wait();
  thread_.join();
}

bool CFLooper::IsCurrentThreadLooper() const {
  return std::this_thread::get_id() == thread_.get_id();
}

CFPromise<void> CFLooper::DispatchAsync(const Closure& macro_task) {
  return CFPromise<void>([this, macro_task](auto resolve) {
    CFLatch latch;

    if (IsCurrentThreadLooper()) {
      macro_task();
      latch.Signal();
      resolve();
      return;
    }

    {
      std::scoped_lock lock(mutex_);
      macro_tasks_.emplace_back([macro_task, &latch]() {
        macro_task();
        latch.Signal();
      });
    }
    cv_.notify_one();
    resolve();
  });
}

CFPromise<void> CFLooper::DispatchMicro(const Closure& micro_task) {
  return CFPromise<void>([this, micro_task](auto resolve) {
    CFLatch latch;

    if (IsCurrentThreadLooper()) {
      micro_task();
      latch.Signal();
      resolve();
      return;
    }

    {
      std::scoped_lock lock(mutex_);
      micro_tasks_.emplace_back([micro_task, &latch]() {
        micro_task();
        latch.Signal();
      });
    }
    cv_.notify_one();
    resolve();
  });
}

void CFLooper::Run() {
  std::vector<Closure> macro_tasks;

  while (is_running_) {
    {
      std::unique_lock lock(mutex_);
      cv_.wait(lock, [this] { return !macro_tasks_.empty() || !micro_tasks_.empty(); });
      std::swap(macro_tasks, macro_tasks_);
    }

    ConsumeMicroTasks();

    for (const Closure& macro_task : macro_tasks) {
      ConsumeMicroTasks();
      macro_task();
    }

    macro_tasks.clear();
  }
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

}  // namespace seen

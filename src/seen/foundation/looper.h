/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.
 */

#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <vector>

#include "seen/foundation/class_constraints.h"

namespace seen {

class CFAbstractLooper : public std::enable_shared_from_this<CFAbstractLooper> {
 public:
  using Closure = std::function<void()>;

  virtual bool IsCurrentThreadLooper() const = 0;

  virtual void DispatchAsync(const Closure& macro_task) = 0;
  virtual void DispatchMicro(const Closure& micro_task) = 0;
};
using CFLooperPtr = std::shared_ptr<CFAbstractLooper>;

class CFLooper : public CFAbstractLooper {
 public:
  CFLooper();
  virtual ~CFLooper();
  virtual bool IsCurrentThreadLooper() const override;  // Implemented on each platform.

  void DispatchAsync(const Closure& macro_task) override;
  void DispatchMicro(const Closure& micro_task) override;

 protected:
  virtual void MakeThreadLocalLooper();                // Implemented on each platform.
  virtual void ConsumeTasks(const Closure& consumer);  // Implemented on each platform.

 private:
  bool is_running_;
  std::vector<Closure> macro_tasks_;
  std::vector<Closure> micro_tasks_;
  std::thread thread_;
  std::mutex mutex_;
  std::condition_variable cv_;

  void Start();
  void Stop();
  void ConsumeMicroTasks();

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFLooper);
};

CFLooperPtr CFLooperGetCurrent();
CFLooperPtr CFCreateLooper();
CFLooperPtr CFGetPlatformLooper();  // Implemented on each platform.

}  // namespace seen

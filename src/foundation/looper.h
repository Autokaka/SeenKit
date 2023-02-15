/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.
 */

#pragma once

#include <functional>
#include <memory>
#include <thread>

#include "foundation/class_constraints.h"
#include "foundation/promise.h"

namespace seen {

class CFAbstractLooper : std::enable_shared_from_this<CFAbstractLooper> {
 public:
  using Closure = std::function<void()>;

  virtual bool IsCurrentThreadLooper() const = 0;

  virtual CFPromise<void> DispatchAsync(const Closure& macro_task) = 0;
  virtual CFPromise<void> DispatchMicro(const Closure& micro_task) = 0;
};
using CFLooperPtr = std::shared_ptr<CFAbstractLooper>;

class CFLooper : public CFAbstractLooper {
 public:
  bool IsCurrentThreadLooper() const override;

  CFPromise<void> DispatchAsync(const Closure& macro_task) override;
  CFPromise<void> DispatchMicro(const Closure& micro_task) override;

 private:
  std::thread thread_;
};
CFLooperPtr CFCreateLooper();

class CFPlatformLooper : public CFAbstractLooper {
 public:
  bool IsCurrentThreadLooper() const override;

  CFPromise<void> DispatchAsync(const Closure& macro_task) override;
  CFPromise<void> DispatchMicro(const Closure& micro_task) override;
};
CFLooperPtr CFGetPlatformLooper();

CFPromise<void> CFPlatformLooperDispatchAsync(const std::function<void()>& task);  // Implemented on each platform.
CFPromise<void> CFPlatformLooperDispatchMicro(const std::function<void()>& task);  // Implemented on each platform.

}  // namespace seen

// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <asio.hpp>
#include <list>
#include <memory>
#include <thread>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"
#include "seen/base/types.h"

namespace seen {

class Worker : public std::enable_shared_from_this<Worker> {
 public:
  using Ptr = std::shared_ptr<Worker>;
  using WeakPtr = std::weak_ptr<Worker>;

  static Ptr Create(const char* name);
  static Ptr Platform();
  explicit Worker() = default;
  virtual ~Worker() = default;

  [[nodiscard]] virtual bool IsCurrent() const = 0;
  [[nodiscard]] virtual const char* GetName() const = 0;

  void DispatchAsync(Closure macro_task);
  void DispatchAsync(Closure macro_task, const TimeDelta& time_delta);
  virtual void DispatchAsync(Closure macro_task, const TimePoint& time_point) = 0;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Worker);
};

class WorkerImpl final : public Worker {
 public:
  using TimerPtr = std::shared_ptr<asio::steady_timer>;
  using WorkGuard = asio::executor_work_guard<asio::io_context::executor_type>;

  explicit WorkerImpl(const char* name);
  ~WorkerImpl() override;

  bool IsCurrent() const override;
  const char* GetName() const override;
  void DispatchAsync(Closure macro_task, const TimePoint& time_point) override;

 private:
  std::string name_;
  std::shared_ptr<asio::io_context> io_context_;
  WorkGuard work_guard_;
  std::thread thread_;
  std::list<TimerPtr> timers_;
};

class PlatformWorker final : public Worker {
 public:
  bool IsCurrent() const override;
  const char* GetName() const override;
  void DispatchAsync(Closure macro_task, const TimePoint& time_point) override;
};

}  // namespace seen

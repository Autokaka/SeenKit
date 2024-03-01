// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <queue>
#include <thread>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"
#include "seen/base/types.h"
#include "seen/base/waitable_event.h"

namespace seen {

class Worker;

class WorkerDriver {
 public:
  explicit WorkerDriver() = default;
  virtual ~WorkerDriver() = default;
  virtual void Start(Closure on_started) = 0;
  virtual void Stop() = 0;
  virtual bool IsCurrent() = 0;
  virtual void SetWakeup(const TimePoint& time_point, Closure task) = 0;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(WorkerDriver);
};

class WorkerDriverImpl final : public WorkerDriver {
 public:
  explicit WorkerDriverImpl(const char* name);
  ~WorkerDriverImpl() override;
  void Start(Closure on_started) override;
  void Stop() override;
  bool IsCurrent() override;
  void SetWakeup(const TimePoint& time_point, Closure task) override;

 private:
  struct WakeupTask {
    Closure callback;
    TimePoint target_time;
  };
  struct WakeupTaskCompare {
    bool operator()(const WakeupTask& task_to_push, const WakeupTask& task_in_queue) const {
      return task_to_push.target_time > task_in_queue.target_time;
    }
  };
  using WakeupTaskQueue = std::priority_queue<WakeupTask, std::deque<WakeupTask>, WakeupTaskCompare>;

  struct Context {
    std::optional<bool> is_stopped;
    AutoResetWaitableEvent latch;
    std::recursive_mutex mutex;
    WakeupTaskQueue wakeup_tasks;
    TimePoint NextWakeupTime();
    void RunExpiredTasks();
  };

  std::recursive_mutex mutex_;
  std::string name_;
  std::unique_ptr<std::thread> thread_;
  std::shared_ptr<Context> context_;
};

class PlatformWorkerDriver final : public WorkerDriver {
 public:
  explicit PlatformWorkerDriver();
  ~PlatformWorkerDriver() override;
  void Start(Closure on_started) override;
  void Stop() override;
  bool IsCurrent() override;
  void SetWakeup(const TimePoint& time_point, Closure task) override;
};

}  // namespace seen

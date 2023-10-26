// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/14.

#pragma once

#include <queue>
#include <thread>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"
#include "seen/base/types.h"
#include "seen/base/waitable_event.h"

namespace seen {

class CFWorker;

interface CFWorkerDriver {
 public:
  explicit CFWorkerDriver() = default;
  virtual ~CFWorkerDriver() = default;
  PROTOCOL(void Start(CFClosure on_started));
  PROTOCOL(void Stop());
  PROTOCOL(bool IsCurrent());
  PROTOCOL(void SetWakeup(const TimePoint& time_point, CFClosure task));

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFWorkerDriver);
};

class CFWorkerDriverImpl final : public CFWorkerDriver {
 public:
  explicit CFWorkerDriverImpl(const char* name);
  ~CFWorkerDriverImpl() override;
  void Start(CFClosure on_started) override;
  void Stop() override;
  bool IsCurrent() override;
  void SetWakeup(const TimePoint& time_point, CFClosure task) override;

 private:
  struct WakeupTask {
    CFClosure callback;
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
    CFAutoResetWaitableEvent latch;
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

class CFPlatformWorkerDriver final : public CFWorkerDriver {
 public:
  explicit CFPlatformWorkerDriver();
  ~CFPlatformWorkerDriver() override;
  void Start(CFClosure on_started) override;
  void Stop() override;
  bool IsCurrent() override;
  void SetWakeup(const TimePoint& time_point, CFClosure task) override;
};

}  // namespace seen

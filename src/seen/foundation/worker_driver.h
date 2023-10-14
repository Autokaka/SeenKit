// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/14.

#pragma once

#include <queue>
#include <thread>

#include "seen/foundation/class_ext.h"
#include "seen/foundation/time_point.h"
#include "seen/foundation/types.h"
#include "seen/foundation/waitable_event.h"

namespace seen {

class CFWorker;

interface CFWorkerDriver {
 public:
  using OnStartedCallback = void (*)(const std::shared_ptr<CFWorker>&);

  explicit CFWorkerDriver() = default;
  virtual ~CFWorkerDriver() = default;
  PROTOCOL(void Start(OnStartedCallback on_started, const std::shared_ptr<CFWorker>& worker));
  PROTOCOL(void Stop());
  PROTOCOL(bool IsCurrent() const);
  PROTOCOL(void SetWakeup(const TimePoint& time_point, CFClosure task));

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFWorkerDriver);
};

class CFWorkerDriverImpl final : public CFWorkerDriver {
 public:
  explicit CFWorkerDriverImpl(const char* name);
  ~CFWorkerDriverImpl() override;
  void Start(OnStartedCallback on_started, const std::shared_ptr<CFWorker>& worker) override;
  void Stop() override;
  [[nodiscard]] bool IsCurrent() const override;
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
    bool is_running;
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
  void Start(OnStartedCallback on_started, const std::shared_ptr<CFWorker>& worker) override;
  void Stop() override;
  [[nodiscard]] bool IsCurrent() const override;
  void SetWakeup(const TimePoint& time_point, CFClosure task) override;
};

}  // namespace seen

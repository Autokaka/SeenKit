// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <queue>

#include "seen/foundation/class_constraints.h"
#include "seen/foundation/time_point.h"

namespace seen {

class CFWorkerTrait {
 public:
  explicit CFWorkerTrait() = default;
  virtual ~CFWorkerTrait() = default;
  virtual void Start() = 0;
  virtual void Stop() = 0;
  [[nodiscard]] virtual bool IsHost() const = 0;
  virtual void SetWakeup(const TimePoint& time_point, void (*on_wake_up)(void*), void* user_data) = 0;
};

class CFWorker final {
 public:
  DISALLOW_COPY_ASSIGN_AND_MOVE(CFWorker);
  using Closure = std::function<void()>;
  using Ptr = std::shared_ptr<CFWorker>;
  using Weak = std::weak_ptr<CFWorker>;

  explicit CFWorker(std::unique_ptr<CFWorkerTrait>&& impl);
  virtual ~CFWorker();
  [[nodiscard]] bool IsHost() const;

  void DispatchAsync(const Closure& macro_task);
  void DispatchAsync(const Closure& macro_task, const TimeDelta& time_delta);
  void DispatchAsync(const Closure& macro_task, const TimePoint& time_point);

 private:
  static void RunExpiredTasksNow(void* worker);
  void RunExpiredTasksNow();

  struct TaskInfo {
    Closure task;
    TimePoint target_time;
    bool operator()(const TaskInfo& ti1, const TaskInfo& ti2) const { return ti1.target_time > ti2.target_time; }
  };
  std::priority_queue<TaskInfo> tasks_;
  std::mutex tasks_mutex_;
  std::unique_ptr<CFWorkerTrait> impl_;
};

CFWorker::Ptr GetPlatformWorker();
CFWorker::Ptr CreateWorker(const std::string& name);

}  // namespace seen

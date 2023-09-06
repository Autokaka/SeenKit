// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/29.

#pragma once

#include <mutex>
#include <thread>

#include "seen/foundation/worker.h"

namespace seen {

class CFStdWorker final : public CFWorkerTrait {
 public:
  void Start() override;
  void Stop() override;
  [[nodiscard]] bool IsHost() const override;
  void SetWakeup(const TimePoint& time_point, void (*on_wake_up)(void*), void* user_data) override;

 private:
  struct WakeupTask {
    void (*task)(void*);
    void* user_data;
    TimePoint target_time;
    bool operator()(const WakeupTask& wt1, const WakeupTask& wt2) { return wt1.target_time > wt2.target_time; }
  };
  struct WakeupTaskCompare {
    bool operator()(const WakeupTask& ti1, const WakeupTask& ti2) { return ti1.target_time > ti2.target_time; }
  };
  using WakeupTaskQueue = std::priority_queue<WakeupTask, std::deque<WakeupTask>, WakeupTaskCompare>;

  void WaitForNextWakeup();
  void ConsumeWakeupTasksNoLock();

  std::unique_ptr<std::thread> thread_;

  std::mutex mutex_;
  std::condition_variable cv_;
  bool signaled_ = false;
  bool running_ = false;

  WakeupTaskQueue wakeup_tasks_;
};

}  // namespace seen

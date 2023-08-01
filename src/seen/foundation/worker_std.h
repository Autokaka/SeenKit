// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/29.

#pragma once

#include <mutex>
#include <thread>

#include "seen/foundation/worker.h"

namespace seen {

class CFWorkerStd final : public CFWorkerTrait {
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
    bool operator()(const WakeupTask& ws1, const WakeupTask& ws2) const { return ws1.target_time > ws2.target_time; }
  };
  void WaitForNextWakeup();
  void ConsumeWakeupTasksNoLock();

  std::unique_ptr<std::thread> thread_;

  std::mutex mutex_;
  std::condition_variable cv_;
  bool signaled_ = false;
  bool running_ = false;

  std::priority_queue<WakeupTask> wakeup_tasks_;
};

}  // namespace seen

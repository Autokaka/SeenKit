// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/30.

#pragma once

#import <dispatch/dispatch.h>

#include "seen/foundation/worker.h"

namespace seen {

class CFWorkerDarwin final : public CFWorkerTrait {
 public:
  explicit CFWorkerDarwin();
  void Start() override;
  void Stop() override;
  [[nodiscard]] bool IsHost() const override;
  void SetWakeup(const TimePoint& time_point, void (*on_wake_up)(void*), void* user_data) override;

 private:
  dispatch_queue_main_t main_queue_ = nullptr;
};

}  // namespace seen

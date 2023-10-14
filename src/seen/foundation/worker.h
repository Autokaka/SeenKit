// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#pragma once

#include <memory>

#include "seen/foundation/class_ext.h"
#include "seen/foundation/time_point.h"
#include "seen/foundation/types.h"

namespace seen {

class CFWorkerDriver;

class CFWorker final : public std::enable_shared_from_this<CFWorker> {
 public:
  using Ptr = std::shared_ptr<CFWorker>;
  using Weak = std::weak_ptr<CFWorker>;

  static Ptr GetCurrent();
  explicit CFWorker(std::unique_ptr<CFWorkerDriver> driver);
  virtual ~CFWorker();
  [[nodiscard]] bool IsCurrent() const;

  void DispatchAsync(CFClosure macro_task);
  void DispatchAsync(CFClosure macro_task, const TimeDelta& time_delta);
  void DispatchAsync(CFClosure macro_task, const TimePoint& time_point);

 private:
  std::unique_ptr<CFWorkerDriver> driver_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFWorker);
};

CFWorker::Ptr CreateWorker(const char* name);
CFWorker::Ptr GetPlatformWorker();

}  // namespace seen

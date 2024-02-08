// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#pragma once

#include <memory>
#include <string>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"
#include "seen/base/types.h"

namespace seen {

class CFWorkerDriver;

class CFWorker final : public std::enable_shared_from_this<CFWorker> {
 public:
  using Ptr = std::shared_ptr<CFWorker>;
  using WeakPtr = std::weak_ptr<CFWorker>;

  static Ptr Create(const char* name);
  static Ptr GetCurrent();
  explicit CFWorker(const char* name, std::unique_ptr<CFWorkerDriver> driver);
  virtual ~CFWorker();
  [[nodiscard]] bool IsCurrent() const;
  [[nodiscard]] std::string GetName() const;

  void DispatchAsync(CFClosure macro_task);
  void DispatchAsync(CFClosure macro_task, const TimeDelta& time_delta);
  void DispatchAsync(CFClosure macro_task, const TimePoint& time_point);

 private:
  std::string name_;
  std::unique_ptr<CFWorkerDriver> driver_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(CFWorker);
};

CFWorker::Ptr GetPlatformWorker();

}  // namespace seen

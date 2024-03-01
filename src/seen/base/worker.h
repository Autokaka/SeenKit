// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/15.

#pragma once

#include <memory>
#include <string>

#include "seen/base/class_ext.h"
#include "seen/base/time_point.h"
#include "seen/base/types.h"

namespace seen {

class WorkerDriver;

class Worker final : public std::enable_shared_from_this<Worker> {
 public:
  using Ptr = std::shared_ptr<Worker>;
  using WeakPtr = std::weak_ptr<Worker>;

  static Ptr Create(const char* name);
  static Ptr GetCurrent();
  explicit Worker(const char* name, std::unique_ptr<WorkerDriver> driver);
  virtual ~Worker();
  [[nodiscard]] bool IsCurrent() const;
  [[nodiscard]] std::string GetName() const;

  void DispatchAsync(Closure macro_task);
  void DispatchAsync(Closure macro_task, const TimeDelta& time_delta);
  void DispatchAsync(Closure macro_task, const TimePoint& time_point);

 private:
  std::string name_;
  std::unique_ptr<WorkerDriver> driver_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Worker);
};

Worker::Ptr GetPlatformWorker();

}  // namespace seen

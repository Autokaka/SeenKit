// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/29.

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "seen/base/class_ext.h"
#include "seen/base/types.h"
#include "seen/base/waitable_event.h"
#include "seen/base/worker.h"

namespace seen {

class WorkerCoordinator final {
 public:
  explicit WorkerCoordinator(CFWorker::Ptr host, const std::vector<CFWorker::Ptr>& cooperators);
  ~WorkerCoordinator();

  void Dispatch(const CFWorker::Ptr& worker, CFClosure task);

 private:
  using LatchPtr = std::shared_ptr<CFAutoResetWaitableEvent>;

  CFWorker::Ptr host_;
  LatchPtr host_latch_;
  std::unordered_map<CFWorker::Ptr, LatchPtr> latch_map_;
  CFClosure current_task_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(WorkerCoordinator);
};

}  // namespace seen

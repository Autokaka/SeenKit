// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
  explicit WorkerCoordinator(Worker::Ptr host, const std::vector<Worker::Ptr>& cooperators);
  ~WorkerCoordinator();

  void Dispatch(const Worker::Ptr& worker, Closure task);

 private:
  using LatchPtr = std::shared_ptr<AutoResetWaitableEvent>;

  Worker::Ptr host_;
  LatchPtr host_latch_;
  std::unordered_map<Worker::Ptr, LatchPtr> latch_map_;
  Closure current_task_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(WorkerCoordinator);
};

}  // namespace seen

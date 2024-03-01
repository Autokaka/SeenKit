// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>

#include "seen/base/class_ext.h"
#include "seen/base/data_channel.h"
#include "seen/base/worker.h"
#include "seen/base/worker_coordinator.h"
#include "seen/bundle.h"
#include "seen/mod/seen.h"
#include "seen/runtime/entry.h"

namespace seen {

class Engine final : public std::enable_shared_from_this<Engine> {
 public:
  using Ptr = std::shared_ptr<Engine>;
  using CreateCallback = std::function<void(const Ptr& engine)>;
  using ExecCallback = std::function<void(bool success)>;

  static void CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback);
  static Ptr Create(const Bundle::Ptr& bundle);

  explicit Engine();
  ~Engine();

  void IsRunning(bool is_running);
  [[nodiscard]] bool IsRunning() const;
  void SetDrawable(const void* view);
  void UpdateDrawable();

 private:
  void ExecEntry(const Bundle::Ptr& bundle, ExecCallback callback);
  void UpdateDrawable(WorkerCoordinator& coordinator);
  mod::Seen::Ptr GetSeen() const;

  Worker::Ptr main_worker_;
  DataChannel::Ptr main_channel_;
  DataChannel::Ptr platform_channel_;
  runtime::StatePtr state_;
  const void* view_;
  void* drawable_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace seen

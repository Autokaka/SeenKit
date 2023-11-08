// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#pragma once

#include <memory>

#include "seen/base/class_ext.h"
#include "seen/base/data_channel.h"
#include "seen/base/time_delta.h"
#include "seen/base/worker.h"

namespace seen {

class Engine final {
 public:
  using Ptr = std::unique_ptr<Engine>;

  explicit Engine(void* handle);
  void Update(const TimeDelta& time_delta, CFClosure on_complete = nullptr);
  [[nodiscard]] CFDataChannel::Ptr GetChannel() const;

 private:
  std::shared_ptr<void> renderer_;
  CFWorker::Ptr main_worker_;
  CFDataChannel::Ptr main_channel_;
  CFDataChannel::Ptr platform_channel_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace seen

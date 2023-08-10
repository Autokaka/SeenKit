// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#pragma once

#include <LLGL/Texture.h>
#include <functional>

#include "seen/foundation/class_constraints.h"
#include "seen/foundation/data_channel.h"
#include "seen/foundation/time_delta.h"
#include "seen/foundation/worker.h"

namespace seen {

class Engine final {
 public:
  DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
  using Ptr = std::unique_ptr<Engine>;
  using EngineDidUpdateCallback = std::function<void(const Engine&, const LLGL::Texture&)>;

  explicit Engine(const CFData::Ptr& bundle_data);
  void Update(const TimeDelta& time_delta, const EngineDidUpdateCallback& callback = nullptr);
  [[nodiscard]] CFDataChannel::Ptr CreateChannel() const;

 private:
  static const char* kMainWorkerName;
  CFWorker::Ptr worker_;
};

}  // namespace seen

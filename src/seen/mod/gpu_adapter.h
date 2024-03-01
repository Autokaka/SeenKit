// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>

#include "seen/base/class_ext.h"
#include "seen/mod/object.h"

namespace seen::mod {

class GPUAdapter final : public Object {
 public:
  using Ptr = std::shared_ptr<GPUAdapter>;

  struct PowerPref final {
    using Type = const char*;
    static constexpr Type kLowPower = "low-power";
    static constexpr Type kHighPerformance = "high-performance";
  };
  static bool IsPowerPref(const std::string& maybe);

  static Ptr Create(WGPUAdapter adapter);
  explicit GPUAdapter(WGPUAdapter adapter);
  ~GPUAdapter() override;

 private:
  WGPUAdapter adapter_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPUAdapter);
};

}  // namespace seen::mod

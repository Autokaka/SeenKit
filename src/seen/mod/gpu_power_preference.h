// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <string>

namespace seen::mod {

struct GPUPowerPreference {
  using Type = const char*;
  static constexpr Type kLowPower = "low-power";
  static constexpr Type kHighPerformance = "high-performance";

  static WGPUPowerPreference ToWGPU(const std::string& preference) {
    if (preference == kLowPower) {
      return WGPUPowerPreference_LowPower;
    }
    if (preference == kHighPerformance) {
      return WGPUPowerPreference_HighPerformance;
    }
    return WGPUPowerPreference_Undefined;
  }

  static WGPUPowerPreference ToWGPU(const char* preference) {
    return ToWGPU(std::string(preference != nullptr ? preference : ""));
  }
};

static bool IsGPUPowerPreference(const std::string& some) {
  return some == GPUPowerPreference::kLowPower ||  //
         some == GPUPowerPreference::kHighPerformance;
}

static bool IsGPUPowerPreference(const char* some) {
  return some != nullptr && IsGPUPowerPreference(std::string(some));
}

}  // namespace seen::mod

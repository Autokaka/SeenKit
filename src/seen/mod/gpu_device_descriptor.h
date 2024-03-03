// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>
#include <vector>

#include "seen/mod/gpu_feature_name.h"
#include "seen/mod/gpu_supported_limits.h"

namespace seen::mod {

struct GPUQueueDescriptor {
  std::string name;

  static WGPUQueueDescriptor ToWGPU(const GPUQueueDescriptor& desc) {
    WGPUQueueDescriptor wgpu_desc;
    std::memset(&wgpu_desc, 0, sizeof(WGPUQueueDescriptor));
    wgpu_desc.label = desc.name.c_str();
    return wgpu_desc;
  }
};

struct GPUDeviceDescriptor {
  std::string label;
  std::vector<GPUFeatureName::Type> required_features;
  std::unordered_map<std::string, std::uint32_t> required_limits;
  GPUQueueDescriptor default_queue;

  static WGPUDeviceDescriptor ToWGPU(const GPUDeviceDescriptor& desc) {
    WGPUDeviceDescriptor wgpu_desc;
    std::memset(&wgpu_desc, 0, sizeof(WGPUDeviceDescriptor));
    // name
    wgpu_desc.label = desc.label.c_str();
    // features
    wgpu_desc.requiredFeatureCount = desc.required_features.size();
    std::vector<WGPUFeatureName> wgpu_features;
    wgpu_features.reserve(wgpu_desc.requiredFeatureCount);
    for (const auto& feature : desc.required_features) {
      wgpu_features.emplace_back(GPUFeatureName::ToWGPU(feature));
    }
    wgpu_desc.requiredFeatures = wgpu_features.data();
    // limits
    auto limits = GPUSupportedLimits::ToWGPU(desc.required_limits);
    WGPURequiredLimits wgpu_limits;
    std::memset(&wgpu_limits, 0, sizeof(WGPURequiredLimits));
    wgpu_limits.limits = limits;
    wgpu_desc.requiredLimits = &wgpu_limits;
    // queue
    wgpu_desc.defaultQueue = GPUQueueDescriptor::ToWGPU(desc.default_queue);
    return wgpu_desc;
  }
};

}  // namespace seen::mod

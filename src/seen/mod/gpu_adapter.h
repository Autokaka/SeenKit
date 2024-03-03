// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "seen/base/class_ext.h"
#include "seen/mod/gpu_adapter_info.h"
#include "seen/mod/gpu_device.h"
#include "seen/mod/gpu_device_descriptor.h"
#include "seen/mod/gpu_feature_name.h"
#include "seen/mod/gpu_supported_limits.h"
#include "seen/mod/object.h"

namespace seen::mod {

using GPUSupportedFeatures = std::set<GPUFeatureName::Type>;

using GPURequestDeviceCallback = std::function<void(const GPUDevice::Ptr& device)>;

using GPUUnmaskHints = std::vector<std::string>;

using GPURequestAdapterInfoCallback = std::function<void(const GPUAdapterInfo::Ptr& adapter_info)>;

class GPUAdapter final : public Object {
 public:
  using Ptr = std::shared_ptr<GPUAdapter>;

  static Ptr Create(WGPUAdapter adapter);
  explicit GPUAdapter(WGPUAdapter adapter);
  ~GPUAdapter() override;

  GPUSupportedFeatures GetFeatures() const;
  GPUSupportedLimits GetLimits() const;
  bool IsFallbackAdapter() const;

  void RequestDevice(const GPUDeviceDescriptor& desc, GPURequestDeviceCallback callback);
  void RequestAdapterInfo(const GPURequestAdapterInfoCallback& callback);

 private:
  WGPUAdapter adapter_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPUAdapter);
};

}  // namespace seen::mod

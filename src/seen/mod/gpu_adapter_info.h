// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>
#include <string>

namespace seen::mod {

struct GPUAdapterInfo {
  using Ptr = std::shared_ptr<GPUAdapterInfo>;

  std::string vendor;
  std::string architecture;
  std::string device;
  std::string description;

  static GPUAdapterInfo FromWGPU(const WGPUAdapterProperties& props) {
    return {
        .vendor = props.vendorName,
        .architecture = props.architecture,
        .device = props.name,
        .description = props.driverDescription,
    };
  }
};

}  // namespace seen::mod

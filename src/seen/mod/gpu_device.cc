// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/mod/gpu_device.h"

namespace seen::mod {

GPUDevice::Ptr GPUDevice::Create(WGPUDevice device) {
  return std::make_shared<GPUDevice>(device);
}

GPUDevice::GPUDevice(WGPUDevice device) : Object(Object::Name::kGPUDevice), device_(device) {}

GPUDevice::~GPUDevice() {
  wgpuDeviceRelease(device_);
}

}  // namespace seen::mod

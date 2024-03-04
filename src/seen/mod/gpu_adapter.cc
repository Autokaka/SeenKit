// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/mod/gpu_adapter.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"

namespace seen::mod {

GPUAdapter::Ptr GPUAdapter::Create(WGPUAdapter adapter) {
  return std::make_shared<GPUAdapter>(adapter);
}

GPUAdapter::GPUAdapter(WGPUAdapter adapter) : Object(Object::Name::kGPUAdapter), adapter_(adapter) {
  SEEN_ASSERT(adapter_);
  SEEN_DEBUG("Create GPU adapter.");
}

GPUAdapter::~GPUAdapter() {
  SEEN_DEBUG("Release GPU adapter.");
  wgpuAdapterRelease(adapter_);
}

GPUSupportedFeatures GPUAdapter::GetFeatures() const {
  auto count = wgpuAdapterEnumerateFeatures(adapter_, nullptr);
  std::vector<WGPUFeatureName> wgpu_features;
  wgpu_features.reserve(count);
  wgpuAdapterEnumerateFeatures(adapter_, wgpu_features.data());
  std::set<GPUFeatureName::Type> features;
  for (auto wgpu_feature : wgpu_features) {
    if (const char* feature = GPUFeatureName::FromWGPU(wgpu_feature)) {
      features.emplace(feature);
    }
  }
  return features;
}

GPUSupportedLimits GPUAdapter::GetLimits() const {
  WGPUSupportedLimits wgpu_limits;
  auto success = wgpuAdapterGetLimits(adapter_, &wgpu_limits);
  SEEN_ASSERT(success);
  return {wgpu_limits.limits};
}

bool GPUAdapter::IsFallbackAdapter() const {
  WGPUAdapterProperties props;
  wgpuAdapterGetProperties(adapter_, &props);
  return props.adapterType != WGPUAdapterType_DiscreteGPU &&  //
         props.adapterType != WGPUAdapterType_IntegratedGPU;
}

void GPUAdapter::RequestDevice(const GPUDeviceDescriptor& desc, GPURequestDeviceCallback callback) {
  auto wgpu_desc = GPUDeviceDescriptor::ToWGPU(desc);
  auto* callback_ptr = new GPURequestDeviceCallback(std::move(callback));
  auto c_callback = [](WGPURequestDeviceStatus status, WGPUDevice device, char const* message, void* user_data) {
    auto* callback_ptr = reinterpret_cast<GPURequestDeviceCallback*>(user_data);
    DeferredTask defer([callback_ptr]() { delete callback_ptr; });
    auto success = status == WGPURequestDeviceStatus_Success && device != nullptr;
    (*callback_ptr)(success ? GPUDevice::Create(device) : nullptr);
  };
  wgpuAdapterRequestDevice(adapter_, &wgpu_desc, std::move(c_callback), callback_ptr);
}

void GPUAdapter::RequestDevice(GPURequestDeviceCallback callback) {
  RequestDevice({}, std::move(callback));
}

void GPUAdapter::RequestAdapterInfo(const GPURequestAdapterInfoCallback& callback) {
  WGPUAdapterProperties wgpu_props;
  wgpuAdapterGetProperties(adapter_, &wgpu_props);
  auto props = GPUAdapterInfo::FromWGPU(wgpu_props);
  callback(std::make_shared<GPUAdapterInfo>(props));
}

}  // namespace seen::mod

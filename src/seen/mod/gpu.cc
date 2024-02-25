// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/gpu.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/mod/gpu_adapter.h"

namespace seen::mod {

const GPU::RequestAdapterOptions GPU::RequestAdapterOptions::kDefault = {
    .power_preference = nullptr,
};

GPU::Ptr GPU::Create() {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu = wgpuCreateInstance(&desc);
  return wgpu != nullptr ? std::make_shared<GPU>(wgpu) : nullptr;
}

GPU::GPU(WGPUInstance wgpu) : Object(Object::Name::kGPU), wgpu_(wgpu) {
  SEEN_DEBUG("Create GPU instance.");
  SEEN_ASSERT(wgpu);
}

GPU::~GPU() {
  SEEN_DEBUG("Release GPU instance.");
  wgpuInstanceRelease(wgpu_);
}

void GPU::RequestAdapter(const RequestAdapterOptions& options, const RequestAdapterCallback& callback) {
  DoRequestAdapter(options, callback);
}

void GPU::RequestAdapter(const RequestAdapterCallback& callback) {
  DoRequestAdapter({.power_preference = nullptr}, callback);
}

void GPU::DoRequestAdapter(const RequestAdapterOptions& options, const RequestAdapterCallback& callback) {
  SEEN_ASSERT(callback);

  WGPURequestAdapterOptions wgpu_options;
  wgpu_options.nextInChain = nullptr;
  // TODO(Autokaka): Get surface from system.
  wgpu_options.compatibleSurface = nullptr;
  wgpu_options.powerPreference = WGPUPowerPreference_Undefined;
  wgpu_options.forceFallbackAdapter = 0;
  wgpu_options.backendType = WGPUBackendType_Undefined;
  if (options.power_preference == GPUAdapter::PowerPref::kLowPower) {
    wgpu_options.powerPreference = WGPUPowerPreference_LowPower;
  } else if (options.power_preference == GPUAdapter::PowerPref::kHighPerformance) {
    wgpu_options.powerPreference = WGPUPowerPreference_HighPerformance;
  }
  auto* callback_ptr = new RequestAdapterCallback(callback);
  auto c_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const*, void* user_data) {
    auto* callback_ptr = reinterpret_cast<RequestAdapterCallback*>(user_data);
    CFDeferredTask defer([callback_ptr]() { delete callback_ptr; });
    auto success = status == WGPURequestAdapterStatus_Success && adapter != nullptr;
    (*callback_ptr)(success ? std::make_shared<GPUAdapter>(adapter) : nullptr);
  };
  wgpuInstanceRequestAdapter(wgpu_, &wgpu_options, c_callback, callback_ptr);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
TGPUTextureFormat GPU::GetPreferredDrawableFormat() const {
  // TODO(Autokaka): Get surface from system.
  auto format = wgpuSurfaceGetPreferredFormat(nullptr, nullptr);
  return GPUTextureFormat::From(format);
}

}  // namespace seen::mod

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/gpu.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/mod/gpu_adapter.h"

namespace seen::mod {

GPU::Ptr GPU::Create(const void* drawable) {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu = wgpuCreateInstance(&desc);
  return wgpu != nullptr ? std::make_shared<GPU>(wgpu, drawable) : nullptr;
}

GPU::GPU(WGPUInstance wgpu, const void* drawable) : Object(Object::Name::kGPU), wgpu_(wgpu), drawable_(drawable) {
  SEEN_DEBUG("Create GPU instance.");
  SEEN_ASSERT(wgpu);
}

GPU::~GPU() {
  SEEN_DEBUG("Release GPU instance.");
  wgpuInstanceRelease(wgpu_);
}

void GPU::RequestAdapter(const RequestAdapterOptions& options, RequestAdapterCallback callback) {
  DoRequestAdapter(options, std::move(callback));
}

void GPU::RequestAdapter(const RequestAdapterCallback& callback) {
  DoRequestAdapter({}, callback);
}

void GPU::DoRequestAdapter(const RequestAdapterOptions& options, RequestAdapterCallback callback) {
  SEEN_ASSERT(callback);
  if (drawable_ == nullptr) {
    callback(nullptr);
    return;
  }
  auto surface = GPUAdapter::SurfacePtr(pal::seen_surface_create(wgpu_, drawable_), wgpuSurfaceRelease);
  WGPURequestAdapterOptions wgpu_options;
  wgpu_options.nextInChain = nullptr;
  wgpu_options.compatibleSurface = surface.get();
  wgpu_options.powerPreference = WGPUPowerPreference_Undefined;
  wgpu_options.forceFallbackAdapter = 0;
  wgpu_options.backendType = WGPUBackendType_Undefined;
  if (options.power_preference == GPUAdapter::PowerPref::kLowPower) {
    wgpu_options.powerPreference = WGPUPowerPreference_LowPower;
  } else if (options.power_preference == GPUAdapter::PowerPref::kHighPerformance) {
    wgpu_options.powerPreference = WGPUPowerPreference_HighPerformance;
  }
  struct Context {
    GPUAdapter::SurfacePtr surface;
    RequestAdapterCallback callback;
  };
  auto* context = new Context{std::move(surface), std::move(callback)};
  auto c_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const*, void* user_data) {
    auto* context = reinterpret_cast<Context*>(user_data);
    CFDeferredTask defer([context]() { delete context; });
    auto success = status == WGPURequestAdapterStatus_Success && adapter != nullptr;
    context->callback(success ? GPUAdapter::Create(adapter, context->surface) : nullptr);
  };
  wgpuInstanceRequestAdapter(wgpu_, &wgpu_options, c_callback, context);
}

}  // namespace seen::mod

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/gpu.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/pal/pal.h"

namespace seen::mod {

GPU::Ptr GPU::Create() {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu = wgpuCreateInstance(&desc);
  return wgpu != nullptr ? std::make_shared<GPU>(wgpu) : nullptr;
}

GPU::GPU(WGPUInstance wgpu) : Object(Object::Name::kGPU), wgpu_(wgpu), drawable_ref_(nullptr), drawable_(nullptr) {
  SEEN_DEBUG("Create GPU instance.");
  SEEN_ASSERT(wgpu_);
  drawable_ref_.OnNext([this](const void* drawable) {
    if (drawable == nullptr) {
      drawable_ = nullptr;
      surface_ = nullptr;
    } else {
      surface_ = Drawable::Surface(pal::gpu_surface_create(wgpu_, drawable), wgpuSurfaceRelease);
      drawable_ = Drawable::Create(drawable, surface_);
    }
  });
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
  if (!surface_) {
    callback(nullptr);
    return;
  }
  WGPURequestAdapterOptions wgpu_options;
  wgpu_options.nextInChain = nullptr;
  wgpu_options.compatibleSurface = surface_.get();
  wgpu_options.powerPreference = WGPUPowerPreference_Undefined;
  wgpu_options.forceFallbackAdapter = 0;
  wgpu_options.backendType = WGPUBackendType_Undefined;
  if (options.power_preference == GPUAdapter::PowerPref::kLowPower) {
    wgpu_options.powerPreference = WGPUPowerPreference_LowPower;
  } else if (options.power_preference == GPUAdapter::PowerPref::kHighPerformance) {
    wgpu_options.powerPreference = WGPUPowerPreference_HighPerformance;
  }
  auto* callback_ptr = new RequestAdapterCallback(std::move(callback));
  auto c_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const*, void* user_data) {
    auto* callback = reinterpret_cast<RequestAdapterCallback*>(user_data);
    DeferredTask defer([callback]() { delete callback; });
    auto success = status == WGPURequestAdapterStatus_Success && adapter != nullptr;
    (*callback)(success ? GPUAdapter::Create(adapter) : nullptr);
  };
  wgpuInstanceRequestAdapter(wgpu_, &wgpu_options, c_callback, callback_ptr);
}

GPUTextureFormat::Type GPU::GetPreferredTextureFormat() const {
  return pal::gpu_get_preferred_texture_format(wgpu_);
}

}  // namespace seen::mod

// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <cstring>

#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/mod/gpu.h"
#include "seen/pal/pal.h"

namespace seen::mod {

WGPURequestAdapterOptions GPURequestAdapterOptions::ToWGPU(const GPURequestAdapterOptions& options,
                                                           WGPUSurface surface) {
  WGPURequestAdapterOptions wgpu_options;
  std::memset(&wgpu_options, 0, sizeof(WGPURequestAdapterOptions));
  wgpu_options.compatibleSurface = surface;
  wgpu_options.powerPreference = GPUPowerPreference::ToWGPU(options.power_preference);
  wgpu_options.forceFallbackAdapter = static_cast<WGPUBool>(options.force_fallback_adapter);
  wgpu_options.backendType = pal::gpu_get_preferred_backend();
  return wgpu_options;
}

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

void GPU::RequestAdapter(const GPURequestAdapterOptions& options, GPURequestAdapterCallback callback) {
  DoRequestAdapter(options, std::move(callback));
}

void GPU::RequestAdapter(const GPURequestAdapterCallback& callback) {
  // wgpuSurfaceGetCapabilities(WGPUSurface surface, WGPUAdapter adapter, WGPUSurfaceCapabilities *capabilities);
  DoRequestAdapter({}, callback);
}

void GPU::DoRequestAdapter(const GPURequestAdapterOptions& options, GPURequestAdapterCallback callback) {
  SEEN_ASSERT(callback);
  if (!surface_) {
    callback(nullptr);
    return;
  }
  auto wgpu_options = GPURequestAdapterOptions::ToWGPU(options, surface_.get());
  auto* callback_ptr = new GPURequestAdapterCallback(std::move(callback));
  auto c_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const*, void* user_data) {
    auto* callback_ptr = reinterpret_cast<GPURequestAdapterCallback*>(user_data);
    DeferredTask defer([callback_ptr]() { delete callback_ptr; });
    auto success = status == WGPURequestAdapterStatus_Success && adapter != nullptr;
    (*callback_ptr)(success ? GPUAdapter::Create(adapter) : nullptr);
  };
  wgpuInstanceRequestAdapter(wgpu_, &wgpu_options, std::move(c_callback), callback_ptr);
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
GPUTextureFormat::Type GPU::GetPreferredTextureFormat() const {
  return pal::gpu_get_preferred_texture_format();
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
WGSLLanguageFeatures GPU::GetWGSLLanguageFeatures() const {
  // TODO(Autokaka): Keep it empty for now since Chromium act like that.
  return {};
}

}  // namespace seen::mod

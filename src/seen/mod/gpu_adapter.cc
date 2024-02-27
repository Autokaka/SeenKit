// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/gpu_adapter.h"
#include "seen/base/logger.h"

namespace seen::mod {

bool GPUAdapter::IsPowerPref(const std::string& maybe) {
  return maybe == PowerPref::kLowPower || maybe == PowerPref::kHighPerformance;
}

GPUAdapter::Ptr GPUAdapter::Create(WGPUAdapter adapter, const SurfacePtr& surface) {
  return std::make_shared<GPUAdapter>(adapter, surface);
}

GPUAdapter::GPUAdapter(WGPUAdapter adapter, SurfacePtr surface)
    : Object(Object::Name::kGPUAdapter), adapter_(adapter), surface_(std::move(surface)) {
  SEEN_ASSERT(adapter_ && surface_);
  SEEN_DEBUG("Create GPU adapter.");
}

GPUAdapter::~GPUAdapter() {
  SEEN_DEBUG("Release GPU adapter.");
  wgpuAdapterRelease(adapter_);
}

TGPUTextureFormat GPUAdapter::GetPreferredTextureFormat() const {
  auto format = wgpuSurfaceGetPreferredFormat(surface_.get(), adapter_);
  return GPUTextureFormat::From(format);
}

}  // namespace seen::mod

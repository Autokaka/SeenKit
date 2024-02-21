// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/gpu_adapter.h"
#include "seen/base/logger.h"

namespace seen::mod {

GPUAdapter::PowerPrefT GPUAdapter::PowerPref::kLowPower = "low-power";
GPUAdapter::PowerPrefT GPUAdapter::PowerPref::kHighPerformance = "high-performance";

bool GPUAdapter::IsPowerPref(const std::string& maybe) {
  return maybe == PowerPref::kLowPower || maybe == PowerPref::kHighPerformance;
}

GPUAdapter::GPUAdapter(WGPUAdapter adapter) : adapter_(adapter) {
  SEEN_ASSERT(adapter);
  SEEN_INFO("Create GPU adapter.");
}

GPUAdapter::~GPUAdapter() {
  SEEN_INFO("Release GPU adapter.");
  wgpuAdapterRelease(adapter_);
}

}  // namespace seen::mod

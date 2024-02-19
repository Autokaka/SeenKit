// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/bindings/gpu_adapter.h"
#include "seen/base/logger.h"

namespace seen::runtime {

GPUAdapter::GPUAdapter(WGPUAdapter adapter) : adapter_(adapter) {
  SEEN_ASSERT(adapter);
  SEEN_INFO("Create GPU adapter.");
}

GPUAdapter::~GPUAdapter() {
  SEEN_INFO("Release GPU adapter.");
  wgpuAdapterRelease(adapter_);
}

}  // namespace seen::runtime

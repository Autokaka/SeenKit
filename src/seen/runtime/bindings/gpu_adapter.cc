// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/bindings/gpu_adapter.h"
#include "seen/base/logger.h"

namespace seen::runtime {

GPUAdapter::GPUAdapter(WGPUAdapter adapter)
    : ScriptClass(ScriptClass::ConstructFromCpp<GPUAdapter>{}), adapter_(adapter) {
  SEEN_ASSERT(adapter);
}

GPUAdapter::~GPUAdapter() {
  wgpuAdapterRelease(adapter_);
}

}  // namespace seen::runtime

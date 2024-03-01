// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/mod/gpu_adapter.h"
#include "seen/base/logger.h"

namespace seen::mod {

bool GPUAdapter::IsPowerPref(const std::string& maybe) {
  return maybe == PowerPref::kLowPower || maybe == PowerPref::kHighPerformance;
}

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

}  // namespace seen::mod

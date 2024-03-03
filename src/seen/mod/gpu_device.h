// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>

#include "seen/mod/object.h"

namespace seen::mod {

class GPUDevice : public Object {
 public:
  using Ptr = std::shared_ptr<GPUDevice>;
  static Ptr Create(WGPUDevice device);

  explicit GPUDevice(WGPUDevice device);
  ~GPUDevice() override;

 private:
  WGPUDevice device_;
};

}  // namespace seen::mod

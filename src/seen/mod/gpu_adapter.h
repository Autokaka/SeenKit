// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>

#include "seen/base/class_ext.h"

namespace seen::mod {

class GPUAdapter {
 public:
  using Ptr = std::shared_ptr<GPUAdapter>;

  explicit GPUAdapter(WGPUAdapter adapter);
  ~GPUAdapter();

 private:
  WGPUAdapter adapter_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPUAdapter);
};

}  // namespace seen::mod

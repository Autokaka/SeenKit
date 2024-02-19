// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>
#include <sol/sol.hpp>

#include "seen/base/class_ext.h"
#include "seen/runtime/bindings/gpu_adapter.h"

namespace seen::runtime {

class GPU final {
 public:
  using Ptr = std::shared_ptr<GPU>;
  static GPU::Ptr Create();
  explicit GPU(WGPUInstance wgpu);
  ~GPU();

  GPUAdapter::Ptr RequestAdapter(const sol::variadic_args& args);

 private:
  WGPUInstance wgpu_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPU);
};

}  // namespace seen::runtime

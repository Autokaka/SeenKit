// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <ScriptX/ScriptX.h>
#include <wgpu/wgpu.h>

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace script;

class GPUAdapter : public ScriptClass {
 public:
  using ScriptClass::ScriptClass;

  explicit GPUAdapter(WGPUAdapter adapter);
  ~GPUAdapter() override;

 private:
  WGPUAdapter adapter_;
};

}  // namespace seen::runtime

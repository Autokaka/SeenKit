// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <ScriptX/ScriptX.h>
#include <wgpu/wgpu.h>

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace script;

class GPU : public ScriptClass {
 public:
  using ScriptClass::ScriptClass;

  static GPU* Create();
  explicit GPU(WGPUInstance wgpu);
  ~GPU() override;

  Local<Value> RequestAdapter(const Arguments& args);

 private:
  WGPUInstance wgpu_;
};

}  // namespace seen::runtime

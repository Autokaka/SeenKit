// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/runtime/bindings/gpu.h"
#include "seen/runtime/bindings/gpu_adapter.h"
#include "seen/runtime/bindings/seen.h"

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace script;

void ExportHostAbilities(const EnginePtr& engine) {
  static auto seen = defineClass<Seen>("seen")  //
                         .function("log", Seen::Log)
                         .property("version", Seen::GetVersion)
                         .property("gpu", Seen::GetGPU)
                         .build();

  engine->registerNativeClass(seen);
  static auto gpu = defineClass<GPU>("GPU")  //
                        .nameSpace("seen.GPU")
                        .constructor(nullptr)
                        .instanceFunction("requestAdapter", &GPU::RequestAdapter)
                        .build();
  engine->registerNativeClass(gpu);

  static auto gpu_adapter = defineClass<GPUAdapter>("GPUAdapter")  //
                                .nameSpace("seen.GPUAdapter")
                                .constructor(nullptr)
                                .build();
  engine->registerNativeClass(gpu_adapter);
}

}  // namespace seen::runtime

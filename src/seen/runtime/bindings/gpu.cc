// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/bindings/gpu.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/runtime/bindings/gpu_adapter.h"

namespace seen::runtime {

GPU* GPU::Create() {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu = wgpuCreateInstance(&desc);
  return wgpu != nullptr ? new GPU(wgpu) : nullptr;
}

GPU::GPU(WGPUInstance wgpu) : ScriptClass(ScriptClass::ConstructFromCpp<GPU>{}), wgpu_(wgpu) {
  SEEN_ASSERT(wgpu);
}

GPU::~GPU() {
  wgpuInstanceRelease(wgpu_);
}

Local<Value> GPU::RequestAdapter(const Arguments& args) {
  using Callback = Global<Function>;
  WGPURequestAdapterOptions wgpu_options;
  wgpu_options.nextInChain = nullptr;
  // TODO(AUtokaka): Get surface from system.
  wgpu_options.compatibleSurface = nullptr;
  wgpu_options.powerPreference = WGPUPowerPreference_Undefined;
  wgpu_options.forceFallbackAdapter = 0;
  wgpu_options.backendType = WGPUBackendType_Undefined;
  if (args[0].isObject()) {
    auto options = args[0].asObject();
    auto power_pref = options.get("powerPreference");
    if (power_pref.isString() && power_pref.asString().toString() == "low-power") {
      wgpu_options.powerPreference = WGPUPowerPreference_LowPower;
    } else if (power_pref.isString() && power_pref.asString().toString() == "high-performance") {
      wgpu_options.powerPreference = WGPUPowerPreference_HighPerformance;
    }
  }
  Callback* callback = nullptr;
  if (args[0].isFunction()) {
    callback = new Callback(args[0].asFunction());
  } else if (args[1].isFunction()) {
    callback = new Callback(args[1].asFunction());
  }
  wgpuInstanceRequestAdapter(
      wgpu_, &wgpu_options,
      [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message_, void* user_data) {
        auto* callback = reinterpret_cast<Callback*>(user_data);
        if (callback == nullptr) {
          return;
        }
        CFDeferredTask defer([callback]() { delete callback; });
        if (status != WGPURequestAdapterStatus_Success || adapter == nullptr) {
          callback->get().call();
        } else {
          auto* gpu_adapter = new GPUAdapter(adapter);
          callback->get().call({}, gpu_adapter->getScriptObject());
        }
      },
      callback);
  return {};
}

}  // namespace seen::runtime

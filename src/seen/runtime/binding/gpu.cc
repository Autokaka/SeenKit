// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/binding/gpu.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/runtime/binding/gpu_adapter.h"

namespace seen::runtime {

GPU::Ptr GPU::Create() {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu = wgpuCreateInstance(&desc);
  return wgpu != nullptr ? std::make_shared<GPU>(wgpu) : nullptr;
}

GPU::GPU(WGPUInstance wgpu) : wgpu_(wgpu) {
  SEEN_INFO("Create GPU instance.");
  SEEN_ASSERT(wgpu);
}

GPU::~GPU() {
  SEEN_INFO("Release GPU instance.");
  wgpuInstanceRelease(wgpu_);
}

GPUAdapter::Ptr GPU::RequestAdapter(const sol::variadic_args& args) {
  WGPURequestAdapterOptions wgpu_options;
  wgpu_options.nextInChain = nullptr;
  // TODO(Autokaka): Get surface from system.
  wgpu_options.compatibleSurface = nullptr;
  wgpu_options.powerPreference = WGPUPowerPreference_Undefined;
  wgpu_options.forceFallbackAdapter = 0;
  wgpu_options.backendType = WGPUBackendType_Undefined;
  if (args[0].get_type() == sol::type::table) {
    auto options = args[0].as<sol::table>();
    auto power_pref = options["powerPreference"];
    auto power_pref_is_string = power_pref.get_type() == sol::type::string;
    if (power_pref_is_string && power_pref.get<std::string>() == "low-power") {
      wgpu_options.powerPreference = WGPUPowerPreference_LowPower;
    } else if (power_pref_is_string && power_pref.get<std::string>() == "high-performance") {
      wgpu_options.powerPreference = WGPUPowerPreference_HighPerformance;
    }
  }
  sol::function* callback = nullptr;
  if (args[0].get_type() == sol::type::function) {
    callback = new sol::function(args[0].as<sol::function>());
  } else if (args[1].get_type() == sol::type::function) {
    callback = new sol::function(args[1].as<sol::function>());
  }
  wgpuInstanceRequestAdapter(
      wgpu_, &wgpu_options,
      [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const* message_, void* user_data) {
        auto* callback = reinterpret_cast<sol::function*>(user_data);
        if (callback == nullptr) {
          return;
        }
        CFDeferredTask defer([callback]() { delete callback; });
        if (status == WGPURequestAdapterStatus_Success && adapter != nullptr) {
          callback->call(std::make_shared<GPUAdapter>(adapter));
        } else {
          callback->call();
        }
      },
      callback);
  return {};
}

}  // namespace seen::runtime

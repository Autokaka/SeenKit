// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/09.

#include <wasm_c_api.h>
#include <wgpu/wgpu.h>
#include <cstddef>

#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

namespace {

void log(wasm_exec_env_t env_, const char* message) {
  SEEN_INFO("{}", message);
}

std::size_t get_version_byte_length(wasm_exec_env_t env_) {
  return std::strlen(SEEN_VERSION);
}

void get_version(wasm_exec_env_t env, void* out_version, std::size_t len) {
  auto* module_instance = get_module_inst(env);
  std::memcpy(out_version, SEEN_VERSION, len);
}

WGPUInstance wgpu_create_instance(wasm_exec_env_t env_) {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  return wgpuCreateInstance(&desc);
}

void wgpu_instance_release(wasm_exec_env_t env_, WGPUInstance instance) {
  wgpuInstanceRelease(instance);
}

void wgpu_instance_request_adapter(wasm_exec_env_t env,
                                   WGPUInstance instance,
                                   const WGPURequestAdapterOptions* options,
                                   std::size_t callback,
                                   void* user_data) {
  struct Context {
    wasm_exec_env_t env;
    std::size_t callback;
    void* user_data;
  };
  auto* context = new Context{env, callback, user_data};
  auto c_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, const char* message, void* user_data) {
    auto* context = reinterpret_cast<Context*>(user_data);
    CFDeferredTask defer_context([context]() { delete context; });
    auto* module_instance = get_module_inst(context->env);
    wasm_ref_as_func(nullptr);
    wasm_runtime_call_wasm_a(context->env, nullptr, 1, nullptr, 5, nullptr);
    // wasm_runtime_call_indirect(context->env, context->callback, ...);
  };
  wgpuInstanceRequestAdapter(instance, options, c_callback, context);
}

}  // namespace

std::vector<NativeSymbol> ExportNativeSymbols() {
#define SYMBOL_OF(Symbol, FuncPtr, Sig, ...) \
  { Symbol, reinterpret_cast<void*>(FuncPtr), Sig, __VA_ARGS__ }
  return {
      SYMBOL_OF("log", log, "($)"),
      SYMBOL_OF("get_version_byte_length", get_version_byte_length, "()i"),
      SYMBOL_OF("get_version", get_version, "(*~)"),
      SYMBOL_OF("wgpu_create_instance", wgpu_create_instance, "()i"),
      SYMBOL_OF("wgpu_instance_release", wgpu_instance_release, "(i)"),
      SYMBOL_OF("wgpu_instance_request_adapter", wgpu_instance_request_adapter, "(i*)"),
  };
}

}  // namespace seen::runtime

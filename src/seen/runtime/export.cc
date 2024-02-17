// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/09.

#include <wgpu/wgpu.h>
#include <cstddef>
#include <cstring>

#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/runtime/export.h"
#include "seen/runtime/wasm_ext.h"

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

WGPUInstance wgpu_create_instance(wasm_exec_env_t env) {
  SEEN_INFO("wgpu_create_instance");
  auto* module_inst = wasm_runtime_get_module_inst(env);
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu_inst = wgpuCreateInstance(&desc);
  SEEN_ASSERT(wasm_externref_set_cleanup(module_inst, wgpu_inst, [](void* ptr) {
    SEEN_INFO("wgpu_instance_release");
    wgpuInstanceRelease(static_cast<WGPUInstance>(ptr));
  }));
  return wgpu_inst;
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
    auto* context = static_cast<Context*>(user_data);
    CFDeferredTask defer([context]() { delete context; });
    wasm_val_t args[] = {
        wasm_i32_val(status),
        wasm_externref_val(adapter),
        wasm_ptr_val(message),
        wasm_i32_val(std::strlen(message)),
        wasm_ptr_val(context->user_data),
    };
    wasm_runtime_call_indirect_a(context->env, context->callback, sizeof(args) / sizeof(wasm_val_t), args);
  };
  wgpuInstanceRequestAdapter(instance, options, c_callback, context);
}

}  // namespace

std::vector<NativeSymbol> ExportNativeSymbols() {
  return {
      wasm_symbol<log>("log", "($)"),
      wasm_symbol<get_version_byte_length>("get_version_byte_length", "()i"),
      wasm_symbol<get_version>("get_version", "(*~)"),
      wasm_symbol<wgpu_create_instance>("wgpu_create_instance", "()r"),
      wasm_symbol<wgpu_instance_request_adapter>("wgpu_instance_request_adapter", "(rrii)"),
  };
}

}  // namespace seen::runtime

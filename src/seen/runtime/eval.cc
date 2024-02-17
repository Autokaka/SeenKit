// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/07.

#include <wasm_export.h>
#include <memory>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/eval.h"

namespace seen::runtime {

static constexpr auto kStackSize = 8 * 1024;
// NOTE(Autokaka):
// Always require the wasm module to export malloc/free functions.
// TL;DR, force the app to use libc heap instead of app heap.
// Refer to wasm-micro-runtime/doc/memory_tune.md for details.
static constexpr auto kAppHeapSize = 0;
static constexpr auto kErrorMaxLength = 256;

void EvaluateModule(const Module& module) {
  static constexpr char const* tag = "EvaluateModule";
  SEEN_INFO("{} on {}.", tag, CFWorker::GetCurrent()->GetName());

  char error_buf[kErrorMaxLength];
  auto* module_ptr = wasm_runtime_instantiate(module.get(), kStackSize, kAppHeapSize, error_buf, sizeof(error_buf));
  if (module_ptr == nullptr) {
    SEEN_ERROR("{} failed: {}", tag, error_buf);
    return;
  }

  auto module_inst = std::shared_ptr<WASMModuleInstanceCommon>(module_ptr, wasm_runtime_deinstantiate);
  auto* exec_env_ptr = wasm_runtime_create_exec_env(module_inst.get(), kStackSize);
  if (exec_env_ptr == nullptr) {
    SEEN_ERROR("{} failed: broken execution environment!", tag);
    return;
  }

  auto exec_env = std::shared_ptr<WASMExecEnv>(exec_env_ptr, wasm_runtime_destroy_exec_env);
  auto* start_func = wasm_runtime_lookup_wasi_start_function(module_inst.get());
  if (start_func == nullptr) {
    SEEN_ERROR("{} failed: no start function!", tag);
    return;
  }

  auto success = wasm_runtime_call_wasm_a(exec_env.get(), start_func, 0, nullptr, 0, nullptr);
  if (!success) {
    const auto* message = wasm_runtime_get_exception(module_inst.get());
    SEEN_ERROR("{} {}", tag, message ?: "Uncaught error.");
    return;
  }
}

}  // namespace seen::runtime

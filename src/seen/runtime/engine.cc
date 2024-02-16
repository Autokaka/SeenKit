#include "seen/runtime/engine.h"
#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

static constexpr char const* kModuleName = "seen";

Engine::Engine() : native_symbols_(ExportNativeSymbols()) {
  SEEN_INFO("Create WebAssembly Engine on {}.", CFWorker::GetCurrent()->GetName());
  RuntimeInitArgs args;
  std::memset(&args, 0, sizeof(RuntimeInitArgs));
  args.mem_alloc_type = Alloc_With_System_Allocator;
  SEEN_ASSERT(wasm_runtime_full_init(&args));
  wasm_runtime_set_log_level(WASM_LOG_LEVEL_VERBOSE);
  SEEN_ASSERT(wasm_runtime_register_natives(kModuleName, native_symbols_.data(), native_symbols_.size()));
}

Engine::~Engine() {
  SEEN_INFO("Release WebAssembly Engine.");
  wasm_runtime_unregister_natives(kModuleName, native_symbols_.data());
  wasm_runtime_destroy();
}

}  // namespace seen::runtime

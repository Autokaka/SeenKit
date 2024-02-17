#include <cstring>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

Engine::Engine() : native_symbols_(ExportNativeSymbols()) {
  SEEN_INFO("Create WebAssembly Engine on {}.", CFWorker::GetCurrent()->GetName());
  RuntimeInitArgs args;
  std::memset(&args, 0, sizeof(RuntimeInitArgs));
  args.mem_alloc_type = Alloc_With_System_Allocator;
  args.native_module_name = "seen";
  args.native_symbols = native_symbols_.data();
  args.n_native_symbols = native_symbols_.size();
  SEEN_ASSERT(wasm_runtime_full_init(&args));
}

Engine::~Engine() {
  SEEN_INFO("Release WebAssembly Engine.");
  wasm_runtime_destroy();
}

}  // namespace seen::runtime

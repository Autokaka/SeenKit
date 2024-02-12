#include <wasm_export.h>
#include <memory>
#include <mutex>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

EnginePtr GetEngine() {
  // NOTE(Autokaka): The wasm_c_api is incomplete.
  // According to source code, it cannot bind with
  // pointers since it does not make address conversion
  // from wasm_exec_env to the app space.
  // Waiting for wamr to find a better solution, use
  // wasm_export for now.
  static constexpr char* module_name = "seen";
  static auto native_symbols = ExportNativeSymbols();
  static auto* symbols_ptr = const_cast<NativeSymbol*>(native_symbols.data());
  static std::shared_ptr<wasm_engine_t> engine;
  static std::once_flag engine_init_flag;
  std::call_once(engine_init_flag, []() {
    // https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/wasm_c_api.md#fyi
    SEEN_INFO("Lazy load wasm_engine on current process.");
    engine = std::shared_ptr<wasm_engine_t>(wasm_engine_new(), [](wasm_engine_t* engine) {
      SEEN_INFO("Release wasm_engine on current process.");
      wasm_runtime_unregister_natives(module_name, symbols_ptr);
      wasm_engine_delete(engine);
    });
    wasm_runtime_register_natives(module_name, symbols_ptr, native_symbols.size());
  });
  return engine;
}

StorePtr GetTLSStore() {
  thread_local StorePtr store;
  if (store == nullptr) {
    SEEN_INFO("Lazy create thread-local wasm_store on: {}.", CFWorker::GetCurrent()->GetName());
    store = StorePtr(wasm_store_new(GetEngine().get()), [](wasm_store_t* store) {
      SEEN_INFO("Release thread-local wasm_store on: {}.", CFWorker::GetCurrent()->GetName());
      wasm_store_delete(store);
    });
  }
  return store;
}

}  // namespace seen::runtime

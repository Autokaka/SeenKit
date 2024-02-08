#include <memory>
#include <mutex>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/engine.h"

namespace seen::runtime {

EnginePtr GetEngine() {
  static std::shared_ptr<wasm_engine_t> engine;
  static std::once_flag engine_init_flag;
  std::call_once(engine_init_flag, []() {
    // https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/wasm_c_api.md#fyi
    SEEN_INFO("Lazy load wasm_engine on current process.");
    engine = std::shared_ptr<wasm_engine_t>(wasm_engine_new(), [](wasm_engine_t* engine) {
      SEEN_INFO("Release wasm_engine on current process.");
      wasm_engine_delete(engine);
    });
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

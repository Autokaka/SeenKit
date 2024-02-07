// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include <wasm_c_api.h>
#include <fstream>

#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
#include "seen/engine.h"

namespace seen {

namespace {

// clang-format off
// https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/doc/wasm_c_api.md#fyi
std::shared_ptr<wasm_engine_t> GetEngine() {
  static std::shared_ptr<wasm_engine_t> engine;
  static std::once_flag engine_init_flag;
  std::call_once(engine_init_flag, []() {
    engine = std::shared_ptr<wasm_engine_t>(wasm_engine_new(), wasm_engine_delete);
  });
  return engine;
}

std::shared_ptr<wasm_store_t> GetTLSStore() {
  thread_local std::shared_ptr<wasm_store_t> store;
  if (store == nullptr) {
    store = std::shared_ptr<wasm_store_t>(wasm_store_new(GetEngine().get()), wasm_store_delete);
  }
  return store;
}
// clang-format on

std::shared_ptr<wasm_module_t> CreateEntryModule(const Bundle::Ptr& bundle) {
  std::ifstream ifs(bundle->GetEntryPath(), std::ios::binary);
  CFDeferredTask defer_ifs([&ifs]() { ifs.close(); });
  if (!ifs) {
    return nullptr;
  }
  ifs.seekg(0, std::ifstream::end);
  auto length = ifs.tellg();
  ifs.seekg(0, std::ifstream::beg);
  wasm_byte_vec_t bytes;
  wasm_byte_vec_new_uninitialized(&bytes, length);
  CFDeferredTask defer([&bytes]() { wasm_byte_vec_delete(&bytes); });
  ifs.read(bytes.data, length);
  if (!ifs) {
    return nullptr;
  }
  auto store = GetTLSStore();
  if (!wasm_module_validate(store.get(), &bytes)) {
    return nullptr;
  }
  return {wasm_module_new(store.get(), &bytes), wasm_module_delete};
}

thread_local std::shared_ptr<wasm_module_t> tls_wasm_module;

}  // namespace

void Engine::CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback) {
  SEEN_ASSERT(bundle);
  auto engine = std::make_shared<Engine>();
  engine->main_worker_->DispatchAsync([engine, bundle, callback = std::move(callback)]() mutable {
    tls_wasm_module = CreateEntryModule(bundle);
    bool success = tls_wasm_module != nullptr;
    GetPlatformWorker()->DispatchAsync([engine, success, callback = std::move(callback)]() {
      // Post task to ensure the engine will be destroyed on platform thread if failed to create module.
      callback(success ? engine : nullptr);
    });
  });
}

Engine::Ptr Engine::Create(const Bundle::Ptr& bundle) {
  SEEN_ASSERT(bundle);
  auto engine = std::make_shared<Engine>();
  bool success = false;
  CFAutoResetWaitableEvent latch;
  engine->main_worker_->DispatchAsync([bundle, &success, &latch]() {
    tls_wasm_module = CreateEntryModule(bundle);
    success = tls_wasm_module != nullptr;
    latch.Signal();
  });
  latch.Wait();
  return success ? engine : nullptr;
}

Engine::Engine()
    : io_worker_(CFWorker::Create("Seen.IO")),
      main_worker_(CFWorker::Create("Seen.Main")),
      main_channel_(CFDataChannel::Create(main_worker_, platform_channel_)),
      platform_channel_(CFDataChannel::Create(GetPlatformWorker(), main_channel_)) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
}

Engine::~Engine() {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
}

CFDataChannel::Ptr Engine::GetChannel() const {
  return platform_channel_;
}

}  // namespace seen

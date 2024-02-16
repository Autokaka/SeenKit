// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "seen/engine.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
#include "seen/runtime/eval.h"
#include "seen/runtime/module.h"

namespace seen {

thread_local runtime::Module tls_module;

void Engine::CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback) {
  SEEN_ASSERT(bundle);
  auto engine = std::make_shared<Engine>();
  engine->main_worker_->DispatchAsync([engine, bundle, callback = std::move(callback)]() mutable {
    tls_module = runtime::LoadModule(bundle->GetEntryPath());
    bool success = tls_module != nullptr;
    GetPlatformWorker()->DispatchAsync([engine, success, callback = std::move(callback)]() {
      // Post task to ensure the engine will be destroyed on platform thread if failed to create module.
      callback(success ? engine : nullptr);
    });
    if (success) {
      runtime::EvaluateModule(tls_module);
    }
  });
}

Engine::Ptr Engine::Create(const Bundle::Ptr& bundle) {
  SEEN_ASSERT(bundle);
  auto engine = std::make_shared<Engine>();
  bool success = false;
  CFAutoResetWaitableEvent latch;
  engine->main_worker_->DispatchAsync([bundle, &success, &latch]() {
    tls_module = runtime::LoadModule(bundle->GetEntryPath());
    success = tls_module != nullptr;
    latch.Signal();
    if (success) {
      runtime::EvaluateModule(tls_module);
    }
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

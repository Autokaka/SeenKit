// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "seen/engine.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
#include "seen/runtime/entry.h"

namespace seen {

void Engine::CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback) {
  SEEN_ASSERT(bundle);
  auto engine = std::make_shared<Engine>();
  engine->main_worker_->DispatchAsync([engine, bundle, callback = std::move(callback)]() mutable {
    runtime::ExecEntry(bundle->GetEntryPath());
    GetPlatformWorker()->DispatchAsync([engine = std::move(engine), callback = std::move(callback)]() {
      // Post task to ensure the engine will be destroyed on platform thread.
      callback(engine);
    });
  });
}

Engine::Ptr Engine::Create(const Bundle::Ptr& bundle) {
  SEEN_ASSERT(bundle);
  auto engine = std::make_shared<Engine>();
  CFAutoResetWaitableEvent latch;
  engine->main_worker_->DispatchAsync([bundle, &latch]() {
    runtime::ExecEntry(bundle->GetEntryPath());
    latch.Signal();
  });
  latch.Wait();
  return engine;
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

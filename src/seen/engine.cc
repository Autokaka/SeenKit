// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "seen/engine.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
#include "seen/mod/seen.h"
#include "seen/runtime/entry.h"

namespace seen {

void Engine::CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback) {
  auto engine = std::make_shared<Engine>();
  engine->Init(bundle, [callback = std::move(callback), engine](bool success) mutable {
    GetPlatformWorker()->DispatchAsync([callback = std::move(callback), engine = std::move(engine), success]() {
      // Ensure engine is released on platform thread.
      callback(success ? engine : nullptr);
    });
  });
}

Engine::Ptr Engine::Create(const Bundle::Ptr& bundle) {
  auto engine = std::make_shared<Engine>();
  CFAutoResetWaitableEvent latch;
  bool result = false;
  engine->Init(bundle, [&latch, &result](bool success) {
    result = success;
    latch.Signal();
  });
  latch.Wait();
  return result ? engine : nullptr;
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

void Engine::Init(const Bundle::Ptr& bundle, InitCallback callback) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  SEEN_ASSERT(bundle);
  main_worker_->DispatchAsync([bundle, callback = std::move(callback)]() {
    auto success = runtime::ExecEntry(bundle->GetEntryPath());
    callback(success);
  });
}

}  // namespace seen

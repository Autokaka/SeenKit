// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "seen/engine.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
#include "seen/runtime/entry.h"

namespace seen {

void Engine::CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback) {
  auto engine = std::make_shared<Engine>();
  engine->MainInit(bundle, [callback = std::move(callback), engine](bool success) mutable {
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
  engine->MainInit(bundle, [&latch, &result](bool success) {
    result = success;
    latch.Signal();
  });
  latch.Wait();
  return result ? engine : nullptr;
}

Engine::Engine()
    : main_worker_(CFWorker::Create("Seen.Main")),
      main_channel_(CFDataChannel::Create(main_worker_, platform_channel_)),
      platform_channel_(CFDataChannel::Create(GetPlatformWorker(), main_channel_)) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
}

Engine::~Engine() {
  MainRelease();
}

void Engine::MainInit(const Bundle::Ptr& bundle, InitCallback callback) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  SEEN_ASSERT(bundle);
  main_worker_->DispatchAsync([engine = shared_from_this(), bundle, callback = std::move(callback)]() {
    engine->state_ = runtime::ExecEntry(engine->main_worker_, bundle->GetEntryPath());
    callback(engine->state_ != nullptr);
  });
}

void Engine::MainRelease() {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([state = std::move(state_), &latch]() mutable {
    state.reset();
    latch.Signal();
  });
  latch.Wait();
  main_worker_.reset();
}

mod::Seen::Ptr Engine::GetSeen() const {
  return state_->get<mod::Seen::Ptr>("seen");
}

void Engine::IsRunning(bool is_running) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, is_running, &latch]() {
    GetSeen()->is_running_ = is_running;
    latch.Signal();
  });
  latch.Wait();
}

bool Engine::IsRunning() const {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  bool is_running = false;
  main_worker_->DispatchAsync([this, &latch, &is_running]() {
    is_running = GetSeen()->is_running_.Get();
    latch.Signal();
  });
  latch.Wait();
  return is_running;
}

void Engine::Drawable(const void* drawable) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, drawable, &latch]() {
    GetSeen()->drawable_ = drawable;
    latch.Signal();
  });
  latch.Wait();
}

const void* Engine::Drawable() const {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  const void* drawable = nullptr;
  main_worker_->DispatchAsync([this, &drawable, &latch]() {
    drawable = GetSeen()->drawable_.Get();
    latch.Signal();
  });
  latch.Wait();
  return drawable;
}

void Engine::DrawableMetrics(const mod::DrawableMetrics& metrics) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, metrics, &latch]() {
    GetSeen()->drawable_metrics_ = metrics;
    latch.Signal();
  });
  latch.Wait();
}

mod::DrawableMetrics Engine::DrawableMetrics() const {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  CFAutoResetWaitableEvent latch;
  mod::DrawableMetrics metrics;
  main_worker_->DispatchAsync([this, &metrics, &latch]() {
    metrics = GetSeen()->drawable_metrics_.Get();
    latch.Signal();
  });
  latch.Wait();
  return metrics;
}

}  // namespace seen

// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "seen/engine.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
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

void Engine::Init(const Bundle::Ptr& bundle, InitCallback callback) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  SEEN_ASSERT(bundle);
  main_worker_->DispatchAsync([engine = shared_from_this(), bundle, callback = std::move(callback)]() {
    engine->seen_ = runtime::ExecEntry(bundle->GetEntryPath());
    callback(engine->seen_ != nullptr);
  });
}

void Engine::IsRunning(bool is_running) {
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, is_running, &latch]() {
    seen_->is_running = is_running;
    latch.Signal();
  });
  latch.Wait();
}

bool Engine::IsRunning() const {
  CFAutoResetWaitableEvent latch;
  bool is_running = false;
  main_worker_->DispatchAsync([this, &latch, &is_running]() {
    is_running = seen_->is_running.Get();
    latch.Signal();
  });
  latch.Wait();
  return is_running;
}

void Engine::Drawable(const void* drawable) {
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, drawable, &latch]() {
    seen_->drawable = drawable;
    latch.Signal();
  });
  latch.Wait();
}

const void* Engine::Drawable() const {
  CFAutoResetWaitableEvent latch;
  const void* drawable = nullptr;
  main_worker_->DispatchAsync([this, &drawable, &latch]() {
    drawable = seen_->drawable.Get();
    latch.Signal();
  });
  latch.Wait();
  return drawable;
}

void Engine::DrawableMetrics(const mod::DrawableMetrics& metrics) {
  CFAutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, metrics, &latch]() {
    seen_->drawable_metrics = metrics;
    latch.Signal();
  });
  latch.Wait();
}

mod::DrawableMetrics Engine::DrawableMetrics() const {
  CFAutoResetWaitableEvent latch;
  mod::DrawableMetrics metrics;
  main_worker_->DispatchAsync([this, &metrics, &latch]() {
    metrics = seen_->drawable_metrics.Get();
    latch.Signal();
  });
  latch.Wait();
  return metrics;
}

}  // namespace seen

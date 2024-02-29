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

void Engine::SetDrawable(const void* drawable) {
  auto platform_worker = GetPlatformWorker();
  SEEN_ASSERT(platform_worker->IsCurrent());
  if (drawable_ == drawable) {
    return;
  }
  WorkerCoordinator coordinator(platform_worker, {main_worker_});
  coordinator.Dispatch(main_worker_, [this]() {
    auto seen = GetSeen();
    seen->drawable_size_ = {0, 0};
    seen->drawable_ = nullptr;
  });
  if (drawable_ != nullptr) {
    pal::engine_free_drawable(drawable_);
  }
  drawable_ = drawable;
  if (drawable_ != nullptr) {
    pal::engine_alloc_drawable(drawable_);
  }
  MainUpdateDrawable(coordinator);
}

void Engine::UpdateDrawable() {
  auto platform_worker = GetPlatformWorker();
  SEEN_ASSERT(platform_worker->IsCurrent());
  WorkerCoordinator coordinator(platform_worker, {main_worker_});
  MainUpdateDrawable(coordinator);
}

void Engine::MainUpdateDrawable(WorkerCoordinator& coordinator) {
  if (drawable_ == nullptr) {
    return;
  }
  std::int64_t width;
  std::int64_t height;
  pal::engine_update_drawable(drawable_, &width, &height);
  coordinator.Dispatch(main_worker_, [this, &width, &height]() {
    auto seen = GetSeen();
    seen->drawable_ = drawable_;
    seen->drawable_size_ = {width, height};
  });
}

}  // namespace seen

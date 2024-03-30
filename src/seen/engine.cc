// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/engine.h"
#include "seen/base/logger.h"
#include "seen/base/waitable_event.h"
#include "seen/runtime/entry.h"

namespace seen {

void Engine::CreateAsync(const Bundle::Ptr& bundle, CreateCallback callback) {
  auto engine = std::make_shared<Engine>();
  engine->ExecEntry(bundle, [callback = std::move(callback), engine](bool success) mutable {
    Worker::Platform()->DispatchAsync([callback = std::move(callback), engine = std::move(engine), success]() {
      // Ensure engine is released on platform thread.
      callback(success ? engine : nullptr);
    });
  });
}

Engine::Ptr Engine::Create(const Bundle::Ptr& bundle) {
  auto engine = std::make_shared<Engine>();
  AutoResetWaitableEvent latch;
  bool result = false;
  engine->ExecEntry(bundle, [&latch, &result](bool success) {
    result = success;
    latch.Signal();
  });
  latch.Wait();
  return result ? engine : nullptr;
}

Engine::Engine()
    : main_worker_(Worker::Create("Seen.Main")),
      main_channel_(DataChannel::Create(main_worker_, platform_channel_)),
      platform_channel_(DataChannel::Create(Worker::Platform(), main_channel_)),
      state_(nullptr),
      view_(nullptr),
      drawable_(nullptr) {
  SEEN_ASSERT(Worker::Platform()->IsCurrent());
}

Engine::~Engine() {
  IsRunning(false);
  SetDrawable(nullptr);
  AutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([state = std::move(state_), &latch]() mutable {
    state.reset();
    latch.Signal();
  });
  latch.Wait();
  main_worker_.reset();
}

void Engine::ExecEntry(const Bundle::Ptr& bundle, ExecCallback callback) {
  SEEN_ASSERT(Worker::Platform()->IsCurrent());
  SEEN_ASSERT(bundle);
  main_worker_->DispatchAsync([engine = shared_from_this(), bundle, callback = std::move(callback)]() {
    engine->state_ = runtime::ExecEntry(engine->main_worker_, bundle->GetEntryPath());
    callback(engine->state_ != nullptr);
  });
}

mod::Seen::Ptr Engine::GetSeen() const {
  return state_->get<mod::Seen::Ptr>("seen");
}

void Engine::IsRunning(bool is_running) {
  SEEN_ASSERT(Worker::Platform()->IsCurrent());
  AutoResetWaitableEvent latch;
  main_worker_->DispatchAsync([this, is_running, &latch]() {
    GetSeen()->is_running_ = is_running;
    latch.Signal();
  });
  latch.Wait();
}

bool Engine::IsRunning() const {
  SEEN_ASSERT(Worker::Platform()->IsCurrent());
  AutoResetWaitableEvent latch;
  bool is_running = false;
  main_worker_->DispatchAsync([this, &latch, &is_running]() {
    is_running = GetSeen()->is_running_.Get();
    latch.Signal();
  });
  latch.Wait();
  return is_running;
}

void Engine::SetDrawable(const void* view) {
  auto platform_worker = Worker::Platform();
  SEEN_ASSERT(platform_worker->IsCurrent());
  if (view_ == view) {
    return;
  }
  WorkerCoordinator coordinator(platform_worker, {main_worker_});
  if (view_ != nullptr) {
    coordinator.Dispatch(main_worker_, [this]() {
      auto seen = GetSeen();
      seen->client_size_ = {0, 0};
      seen->drawable_ref_ = nullptr;
    });
    pal::engine_free_drawable(view_, drawable_);
  }
  view_ = view;
  if (view_ != nullptr) {
    auto* drawable = pal::engine_alloc_drawable(view_);
    coordinator.Dispatch(main_worker_, [this, drawable]() {
      auto seen = GetSeen();
      seen->drawable_ref_ = drawable;
    });
    UpdateDrawable(coordinator);
  }
}

void Engine::UpdateDrawable() {
  auto platform_worker = Worker::Platform();
  SEEN_ASSERT(platform_worker->IsCurrent());
  WorkerCoordinator coordinator(platform_worker, {main_worker_});
  UpdateDrawable(coordinator);
}

void Engine::UpdateDrawable(WorkerCoordinator& coordinator) {
  if (view_ == nullptr) {
    return;
  }
  float width;
  float height;
  pal::engine_get_drawable_client_size(view_, &width, &height);
  auto pixel_ratio = pal::engine_get_device_pixel_ratio(view_);
  coordinator.Dispatch(main_worker_, [this, &width, &height, &pixel_ratio]() {
    auto seen = GetSeen();
    seen->client_size_ = {width, height};
    seen->device_pixel_ratio_ = pixel_ratio;
  });
}

}  // namespace seen

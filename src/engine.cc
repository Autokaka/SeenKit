// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "engine.h"
#include "seen/pal/pal.h"
#include "seen/scene/animation.h"
#include "seen/scene/scene.h"

namespace seen {

const char* const kMainWorkerName = "Seen.Main";

Engine::Engine(void* context)
    : context_(context, pal::engine_release),
      main_worker_(CFWorker::Create(kMainWorkerName)),
      main_channel_(std::make_shared<CFDataChannel>(main_worker_, platform_channel_)),
      platform_channel_(std::make_shared<CFDataChannel>(GetPlatformWorker(), main_channel_)) {
  // TODO(Autokaka): Remove it after tests.
  main_worker_->DispatchAsync([context = context_]() {
    auto* scene = Scene::GetCurrent();
    scene->Draw(TimeDelta::Zero(), pal::renderer_get_drawable_size(context));
  });
}

CFDataChannel::Ptr Engine::GetChannel() const {
  return platform_channel_;
}

void Engine::Update(const TimeDelta& time_delta, CFClosure on_complete) {
  main_worker_->DispatchAsync([time_delta, context = context_, on_complete = std::move(on_complete)]() {
    // TODO(Autokaka): Check drawable is valid.

    scene::Animation::UpdateAll(time_delta);
    auto* scene = Scene::GetCurrent();
    if (scene->IsDirty()) {
      scene->Draw(time_delta, pal::renderer_get_drawable_size(context));
    }
    if (on_complete) {
      on_complete();
    }
  });
}

}  // namespace seen

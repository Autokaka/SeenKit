// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "engine.h"
#include "seen/pal/pal.h"
#include "seen/scene/animation/animation.h"
#include "seen/scene/scene.h"

namespace seen {

const char* const kMainWorkerName = "Seen.Main";

Engine::Engine(void* handle)
    : renderer_(handle, pal::renderer_release),
      main_worker_(CFWorker::Create(kMainWorkerName)),
      main_channel_(std::make_shared<CFDataChannel>(main_worker_, platform_channel_)),
      platform_channel_(std::make_shared<CFDataChannel>(GetPlatformWorker(), main_channel_)) {
  // TODO(Autokaka): Remove it after tests.
  main_worker_->DispatchAsync([renderer = renderer_]() {
    auto* scene = Scene::GetCurrent();
    // const auto* handle = pal::renderer_drawable_lock(renderer);
    // scene->Draw(pal::renderer_get_drawable_size(handle));
  });
}

CFDataChannel::Ptr Engine::GetChannel() const {
  return platform_channel_;
}

void Engine::Update(const TimeDelta& time_delta, CFClosure on_complete) {
  main_worker_->DispatchAsync([time_delta, renderer = renderer_, on_complete = std::move(on_complete)]() {
    const auto* drawable = pal::renderer_drawable_lock(renderer);
    if (drawable == nullptr) {
      return;
    }

    scene::Animation::UpdateAll(time_delta);
    auto* scene = Scene::GetCurrent();
    scene->Draw(pal::renderer_get_drawable_size(drawable));

    if (on_complete) {
      on_complete();
    }
  });
}

}  // namespace seen

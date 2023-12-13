// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "engine.h"
#include "seen/base/logger.h"
#include "seen/pal/pal.h"
#include "seen/scene/animation/animation.h"
#include "seen/scene/scene.h"

namespace seen {

namespace {

std::vector<scene::Node::Ptr> FlattenNodes(const scene::Node::Ptr& node) {
  std::vector<scene::Node::Ptr> nodes = {node};
  for (auto& child : node->GetChildren()) {
    auto flatten_nodes = FlattenNodes(child);
    nodes.insert(nodes.end(), flatten_nodes.begin(), flatten_nodes.end());
  }
  return nodes;
}

}  // namespace

const char* const kMainWorkerName = "Seen.Main";

Engine::Engine(void* renderer)
    : renderer_(renderer),
      main_worker_(CFWorker::Create(kMainWorkerName)),
      main_channel_(std::make_shared<CFDataChannel>(main_worker_, platform_channel_)),
      platform_channel_(std::make_shared<CFDataChannel>(GetPlatformWorker(), main_channel_)) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  // TODO(Autokaka): Remove it after tests.
  main_worker_->DispatchAsync([renderer = renderer_]() {
    auto* scene = Scene::GetTLS();
    // const auto* handle = pal::renderer_drawable_lock(renderer);
    // scene->Draw(pal::renderer_get_drawable_size(handle));
  });
}

Engine::~Engine() {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  pal::renderer_release(renderer_);
}

CFDataChannel::Ptr Engine::GetChannel() const {
  return platform_channel_;
}

void Engine::Update(const TimeDelta& time_delta, CFClosure on_complete) {
  main_worker_->DispatchAsync([time_delta, renderer = renderer_, on_complete = std::move(on_complete)]() {
    scene::Animation::UpdateAll(time_delta);
    auto* scene = Scene::GetTLS();
    scene->elapsed_time_ = scene->elapsed_time_.Get() + time_delta;
    scene->size_ = pal::renderer_get_drawable_size(renderer);
    if (scene->is_dirty_.Get()) {
      pal::renderer_draw_scene(renderer, scene);
      if (auto root_node = scene->root_node.Get()) {
        auto flatten_nodes = FlattenNodes(root_node);
        for (const auto& child : flatten_nodes) {
          pal::renderer_draw_node(renderer, child);
        }
      }
    }
    if (on_complete) {
      on_complete();
    }
  });
}

}  // namespace seen

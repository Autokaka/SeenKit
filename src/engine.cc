// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "engine.h"
#include "seen/base/logger.h"
#include "seen/pal/pal.h"
#include "seen/scene/animation/animation.h"
#include "seen/scene/component/sprite.h"
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

Engine::Engine(void* renderer)
    : renderer_(renderer),
      io_worker_(CFWorker::Create("Seen.IO")),
      main_worker_(CFWorker::Create("Seen.Main")),
      main_channel_(std::make_shared<CFDataChannel>(main_worker_, platform_channel_)),
      platform_channel_(std::make_shared<CFDataChannel>(GetPlatformWorker(), main_channel_)) {
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());

  // TODO(Autokaka): Remove it after tests...
  main_worker_->DispatchAsync([]() {
    auto* scene = Scene::GetTLS();
    scene->background_color = {1, 0, 0, 1};
    // auto root_node = scene::Node::Create();
    // auto sprite = scene::Sprite::Create();
    // sprite->color = {0, 1, 0, 1};
    // sprite->bounds = Rect::CreateFromOriginAndSize({0, 0}, {10, 10});
    // root_node->component = sprite;
    // auto test_node = scene::Node::Create();
    // root_node->AddChild(test_node);
    // scene->root_node = root_node;
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
  SEEN_ASSERT(GetPlatformWorker()->IsCurrent());
  main_worker_->DispatchAsync([this, time_delta, on_complete = std::move(on_complete)]() {
    scene::Animation::UpdateAll(time_delta);
    auto* scene = Scene::GetTLS();
    scene->elapsed_time_ = scene->elapsed_time_.Get() + time_delta;
    scene->size_ = pal::renderer_get_drawable_size(renderer_);
    if (scene->is_dirty_.Get()) {
      SEEN_INFO("Update scene...");
      pal::renderer_draw_scene(renderer_, scene);
      if (auto root_node = scene->root_node.Get()) {
        auto flatten_nodes = FlattenNodes(root_node);
        for (const auto& child : flatten_nodes) {
          pal::renderer_draw_node(renderer_, child);
        }
      }
      pal::renderer_present_drawable(renderer_);
      scene->is_dirty_ = false;
    }
    GetPlatformWorker()->DispatchAsync(on_complete);
  });
}

}  // namespace seen

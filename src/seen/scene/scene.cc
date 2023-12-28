// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/05.

#include "seen/scene/scene.h"

namespace seen {

Scene* Scene::GetTLS() {
  thread_local Scene::Ptr scene;
  if (!scene) {
    scene = std::make_unique<Scene>();
    scene->Init();
  }
  return scene.get();
}

Scene::Scene() : size(&size_), scale(&scale_), elapsed_time(&elapsed_time_) {}

void Scene::Init() {
  Reset();
  rx::Bind([this]() { is_dirty_ = true; }, size_, scale_, background_color, root_node);
  is_dirty_.OnNext([this](bool is_dirty) {
    if (is_dirty) {
      return;
    }
    if (auto root_node = this->root_node.Get()) {
      root_node->is_dirty_ = false;
    }
  });
}

void Scene::Reset() {
  root_node = nullptr;
  elapsed_time_ = TimeDelta::Zero();
  background_color = {0, 0, 0, 0};
  is_dirty_ = true;
}

}  // namespace seen

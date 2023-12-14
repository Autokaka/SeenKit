// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/05.

#include "seen/scene/scene.h"

namespace seen {

Scene* Scene::GetTLS() {
  thread_local auto scene = std::make_unique<Scene>();
  return scene.get();
}

Scene::Scene() : size(&size_), scale(&scale_), elapsed_time(&elapsed_time_) {
  Reset();
  rx::LinkWithValues([this]() { needs_repaint_ = true; }, size_, scale_, elapsed_time_, background_color, root_node);
}

void Scene::Reset() {
  root_node = nullptr;
  elapsed_time_ = TimeDelta::Zero();
  background_color = {0, 0, 0, 0};
  needs_repaint_ = true;
}

}  // namespace seen

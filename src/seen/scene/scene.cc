// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/05.

#include "seen/scene/scene.h"

namespace seen {

Scene* Scene::GetTLS() {
  thread_local auto scene = std::make_unique<Scene>();
  return scene.get();
}

Scene::Scene() : size(&size_), scale(&scale_), elapsed_time(&elapsed_time_), background_color({0, 0, 0, 0}) {
  rx::LinkWithValues([this]() { is_dirty_ = true; }, size_, scale_, elapsed_time_, background_color, root_node);
}

}  // namespace seen

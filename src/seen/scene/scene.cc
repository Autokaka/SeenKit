// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/05.

#include "seen/scene/scene.h"

namespace seen {

Scene* Scene::GetCurrent() {
  static auto scene = std::make_unique<Scene>();
  return scene.get();
}

glm::vec2 Scene::GetSize() const {
  return size_;
}

void Scene::Draw(const glm::vec2& size) {
  UpdateSizeIfNeeded(size);
  if (is_dirty_) {
  }
}

void Scene::UpdateSizeIfNeeded(const glm::vec2& size) {
  if (size_ != size) {
    size_ = size;
    is_dirty_ = true;
    if (on_size_changed_callback) {
      on_size_changed_callback(size);
    }
  }
}

}  // namespace seen

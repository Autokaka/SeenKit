// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#pragma once

#include <functional>
#include <glm/glm.hpp>

namespace seen {

class Scene {
 public:
  using Ptr = std::unique_ptr<Scene>;
  using OnSizeChangedCallback = std::function<void(const glm::vec2&)>;

  static Scene* GetCurrent();

  // Size
  [[nodiscard]] glm::vec2 GetSize() const;
  OnSizeChangedCallback on_size_changed_callback;

  void Draw(const glm::vec2& size);

 private:
  friend class Engine;

  void UpdateSizeIfNeeded(const glm::vec2& size);

  bool is_dirty_;
  glm::vec2 size_;
};

}  // namespace seen

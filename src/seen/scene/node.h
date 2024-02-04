// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/09.

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "seen/base/class_ext.h"
#include "seen/base/rect.h"
#include "seen/base/rx_value.h"

namespace seen {

class Scene;

}

namespace seen::scene {

class Component;

class Node final : public std::enable_shared_from_this<Node> {
 public:
  friend class Component;
  friend class seen::Scene;
  using Ptr = std::shared_ptr<Node>;
  using WeakPtr = std::weak_ptr<Node>;

  static Ptr Create();
  explicit Node();

  const rx::View<Rect> bounds;
  rx::Value<glm::vec2> scale;
  rx::Value<float> rotation_z;
  rx::Value<glm::vec2> position;

  rx::Value<std::shared_ptr<Component>> component;

  const rx::View<glm::mat3> parent_transform;
  const rx::View<glm::mat3> scene_transform;
  glm::mat3 GetTransform(const Ptr& from_node = nullptr) const;

  bool IsRootNode() const;
  Ptr GetParent() const;
  void RemoveFromParent();
  void AddChild(const Ptr& child);
  [[nodiscard]] std::vector<Ptr> GetChildren() const;

 private:
  void Init();
  void UpdateParentTransform(const glm::vec2& scale, float rotation_z, const glm::vec2& position);
  WeakPtr parent_;
  std::vector<Ptr> children_;
  rx::Value<bool> is_dirty_;
  rx::Value<Rect> bounds_;
  rx::Value<glm::mat3> parent_transform_;
  rx::Value<glm::mat3> scene_transform_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Node);
};

}  // namespace seen::scene

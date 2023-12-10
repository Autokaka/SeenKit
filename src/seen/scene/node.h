// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/09.

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <vector>

#include "seen/base/class_ext.h"
#include "seen/base/rect.h"
#include "seen/base/rx_value.h"

namespace seen::scene {

class Node : public std::enable_shared_from_this<Node> {
 public:
  using Ptr = std::shared_ptr<Node>;
  using WeakPtr = std::weak_ptr<Node>;

  explicit Node();

  rx::Value<Rect> bounds;
  rx::Value<glm::vec2> scale;
  rx::Value<float> rotation_z;
  rx::Value<glm::vec2> position;

  const rx::View<glm::mat3> parent_transform;
  const rx::View<glm::mat3> world_transform;
  glm::mat3 GetTransform(const Ptr& from_node = nullptr) const;

  Ptr GetParent();
  void RemoveFromParent();
  void AddChild(const Ptr& child);
  [[nodiscard]] std::vector<Ptr> GetChildren() const;

 protected:
  rx::Value<bool> is_dirty_;

 private:
  WeakPtr parent_;
  std::vector<Ptr> children_;
  rx::Value<glm::mat3> parent_transform_;
  rx::Value<glm::mat3> world_transform_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Node);
};

}  // namespace seen::scene

// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#pragma once

#include <glm/glm.hpp>

#include "seen/base/class_ext.h"
#include "seen/base/rx_value.h"
#include "seen/base/time_delta.h"
#include "seen/scene/node.h"

namespace seen {

class Scene final {
 public:
  friend class Engine;
  friend class scene::Node;

  using Ptr = std::unique_ptr<Scene>;

  explicit Scene();
  static Scene* GetTLS();

  const rx::View<TimeDelta> elapsed_time;
  const rx::View<glm::vec2> size;
  const rx::View<glm::vec2> scale;
  rx::Value<glm::vec4> background_color;
  rx::Value<scene::Node::Ptr> root_node;

 private:
  void Init();
  void Reset();

  rx::Value<bool> is_dirty_;
  rx::Value<glm::vec2> size_;
  rx::Value<glm::vec2> scale_;
  rx::Value<TimeDelta> elapsed_time_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Scene);
};

}  // namespace seen

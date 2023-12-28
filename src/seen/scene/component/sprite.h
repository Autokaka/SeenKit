// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/28.

#pragma once

#include <glm/glm.hpp>

#include "seen/base/data.h"
#include "seen/base/rect.h"
#include "seen/base/rx_value.h"
#include "seen/scene/component/component.h"

namespace seen::scene {

class Node;

extern const char* const kSpriteClassName;

class Sprite : public Component {
 public:
  using Ptr = std::shared_ptr<Sprite>;

  static Ptr Create();
  static Ptr FromNode(const std::shared_ptr<Node>& node);
  static Ptr FromFile(const char* path_in_sandbox);
  static Ptr FromImageData(const CFData::Ptr& data);
  static Ptr FromPixelData(const CFData::Ptr& data, const glm::vec2& size);

  explicit Sprite();

  rx::Value<glm::vec4> color;
  rx::Value<double> color_blend_factor;
  rx::Value<Rect> bounds;

 private:
  void Init();
};

}  // namespace seen::scene

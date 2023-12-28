// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/28.

#include "seen/scene/component/sprite.h"
#include "seen/scene/node.h"

namespace seen::scene {

const char* const kSpriteClassName = "Sprite";

Sprite::Ptr Sprite::Create() {
  auto sprite = std::make_shared<Sprite>();
  sprite->Init();
  return sprite;
}

Sprite::Ptr Sprite::FromNode(const Node::Ptr& node) {
  // TODO(Autokaka):
  return nullptr;
}

Sprite::Ptr Sprite::FromFile(const char* path_in_sandbox) {
  // TODO(Autokaka):
  return nullptr;
}

Sprite::Ptr Sprite::FromImageData(const CFData::Ptr& data) {
  // TODO(Autokaka):
  return nullptr;
}

Sprite::Ptr Sprite::FromPixelData(const CFData::Ptr& data, const glm::vec2& size) {
  // TODO(Autokaka):
  return nullptr;
}

Sprite::Sprite() : Component(kSpriteClassName), color({0, 0, 0, 0}), color_blend_factor(1.0), bounds(Rect::Zero()) {}

void Sprite::Init() {
  rx::Bind([this]() { RepaintNodes(); }, color, color_blend_factor);
  bounds.OnNext([this](const Rect& new_bouds) { SetNodeBounds(new_bouds); });
}

}  // namespace seen::scene

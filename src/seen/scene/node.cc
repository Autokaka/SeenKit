// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/09.

#include <glm/gtx/matrix_transform_2d.hpp>

#include "seen/scene/node.h"

namespace seen::scene {

Node::Node() : parent_transform(&parent_transform_), world_transform(&world_transform_) {
  bounds.OnUpdate([this](auto) { is_dirty_ = true; });
  // clang-format off
  rx::LinkWithValues([this]() {
    glm::mat3 mat(1.0F);
    mat = glm::translate(mat, position.Get());
    auto current_rotate = rotation_z.Get();
    if (current_rotate != 0.0F) {
      mat = glm::rotate(mat, current_rotate);
    }
    auto current_scale = scale.Get();
    if (current_scale != glm::vec2(1.0F, 1.0F)) {
      mat = glm::scale(mat, current_scale);
    }
    parent_transform_ = mat;
    is_dirty_ = true;
  }, scale, rotation_z, position);
  // clang-format on
  parent_transform_.OnUpdate([this](const glm::mat3& parent_transform) {
    auto parent = GetParent();
    auto world_parent_transform = parent ? parent->world_transform_.Get() : glm::mat3(1.0F);
    world_transform_ = world_parent_transform * parent_transform_.Get();
    is_dirty_ = true;
  });
}

Node::Ptr Node::GetParent() {
  return parent_.lock();
}

void Node::RemoveFromParent() {
  if (auto parent = GetParent()) {
    auto& children = parent->children_;
    children.erase(std::remove(children.begin(), children.end(), shared_from_this()), children.end());
    world_transform_ = parent_transform_;
    parent->is_dirty_ = true;
    parent_.reset();
  }
}

void Node::AddChild(const Ptr& child) {
  children_.emplace_back(child);
  is_dirty_ = true;
}

std::vector<Node::Ptr> Node::GetChildren() const {
  return children_;
}

}  // namespace seen::scene

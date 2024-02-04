// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/09.

#include <glm/gtx/matrix_transform_2d.hpp>

#include "seen/scene/component/component.h"
#include "seen/scene/component/sprite.h"
#include "seen/scene/node.h"
#include "seen/scene/scene.h"

namespace seen::scene {

Node::Ptr Node::Create() {
  auto node = std::make_shared<Node>();
  node->Init();
  return node;
}

Node::Node()
    : bounds(&bounds_),
      parent_transform(&parent_transform_),
      scene_transform(&scene_transform_),
      scale({1, 1}),
      rotation_z(0.0F),
      position({0, 0}),
      is_dirty_(false),
      bounds_(Rect::Zero()) {}

void Node::Init() {
  component.OnNext([this](const Component::Ptr& new_component) {
    if (auto prev_component = this->component.Get()) {
      auto& nodes = prev_component->nodes_;
      // clang-format off
      nodes.erase(std::remove_if(nodes.begin(), nodes.end(), [this](const Node::WeakPtr& node) {
        return node.lock() == shared_from_this();
      }), nodes.end());
      // clang-format on
    }
    if (new_component) {
      new_component->nodes_.emplace_back(shared_from_this());
    }
  });
  is_dirty_.OnNext([this](bool is_dirty) {
    if (is_dirty) {
      if (auto parent = GetParent()) {
        parent->is_dirty_ = true;
      }
      if (IsRootNode()) {
        Scene::GetTLS()->is_dirty_ = true;
      }
    } else {
      for (auto& child : children_) {
        child->is_dirty_ = false;
      }
    }
  });
  bounds_.OnNext([this](auto) { is_dirty_ = true; });
  // clang-format off
  scale.OnNext([this](const glm::vec2& new_scale) { UpdateParentTransform(new_scale, rotation_z.Get(), position.Get()); });
  rotation_z.OnNext([this](float new_rotation_z) { UpdateParentTransform(scale.Get(), new_rotation_z, position.Get()); });
  position.OnNext([this](const glm::vec2& new_position) { UpdateParentTransform(scale.Get(), rotation_z.Get(), new_position); });
  // clang-format on
  parent_transform_.OnNext([this](const glm::mat3& parent_transform) {
    auto parent = GetParent();
    auto world_parent_transform = parent ? parent->scene_transform_.Get() : glm::mat3(1.0F);
    scene_transform_ = world_parent_transform * parent_transform_.Get();
    is_dirty_ = true;
  });
}

bool Node::IsRootNode() const {
  auto root_node = Scene::GetTLS()->root_node.Get();
  return root_node == shared_from_this();
}

Node::Ptr Node::GetParent() const {
  return parent_.lock();
}

void Node::RemoveFromParent() {
  if (auto parent = GetParent()) {
    auto& children = parent->children_;
    children.erase(std::remove(children.begin(), children.end(), shared_from_this()), children.end());
    scene_transform_ = parent_transform_;
    parent->is_dirty_ = true;
    parent_.reset();
  }
}

void Node::AddChild(const Ptr& child) {
  child->RemoveFromParent();
  children_.emplace_back(child);
  is_dirty_ = true;
}

std::vector<Node::Ptr> Node::GetChildren() const {
  return children_;
}

void Node::UpdateParentTransform(const glm::vec2& scale, float rotation_z, const glm::vec2& position) {
  glm::mat3 mat(1.0F);
  mat = glm::translate(mat, position);
  if (rotation_z != 0.0F) {
    mat = glm::rotate(mat, rotation_z);
  }
  if (scale != glm::vec2(1.0F, 1.0F)) {
    mat = glm::scale(mat, scale);
  }
  parent_transform_ = mat;
  is_dirty_ = true;
}

}  // namespace seen::scene

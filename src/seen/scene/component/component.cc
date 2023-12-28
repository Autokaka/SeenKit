#include "seen/scene/component/component.h"
#include "seen/scene/node.h"

namespace seen::scene {

Component::Component(const char* class_name) : class_name(class_name) {}

void Component::RepaintNodes() {
  TraverseNodes([](const Node::Ptr& node) { node->is_dirty_ = true; });
}

void Component::SetNodeBounds(const Rect& bounds) {
  TraverseNodes([bounds](const Node::Ptr& node) { node->bounds_ = bounds; });
}

void Component::TraverseNodes(const NodeTraverser& traverser) {
  for (auto& weak_node : nodes_) {
    if (auto node = weak_node.lock()) {
      traverser(node);
    }
  }
}

}  // namespace seen::scene

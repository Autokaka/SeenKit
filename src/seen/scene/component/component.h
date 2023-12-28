#pragma once

#include <functional>
#include <memory>

#include "seen/base/class_ext.h"
#include "seen/base/rect.h"
#include "seen/base/rx_value.h"

namespace seen::scene {

class Node;

class Component : public std::enable_shared_from_this<Component> {
 public:
  friend class Node;
  using Ptr = std::shared_ptr<Component>;
  using NodeTraverser = std::function<void(const std::shared_ptr<Node>&)>;

  explicit Component(const char* class_name);

  const char* class_name;

 protected:
  void RepaintNodes();
  void SetNodeBounds(const Rect& bounds);

 private:
  void TraverseNodes(const NodeTraverser& traverser);

  std::vector<std::weak_ptr<Node>> nodes_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Component);
};

}  // namespace seen::scene

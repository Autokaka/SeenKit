// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/02.

#pragma once

#include <memory>

namespace seen::scene {

class Node {
 public:
  using Ptr = std::shared_ptr<Node>;
  using WeakPtr = std::weak_ptr<Node>;
};

}  // namespace seen::scene

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/26.

#pragma once

#include <memory>

namespace seen::mod {

struct Object : public std::enable_shared_from_this<Object> {
  using TName = const char*;
  struct Name {
    static constexpr TName kDrawableMetrics = "DrawableMetrics";
    static constexpr TName kFramePacer = "FramePacer";
    static constexpr TName kGPUAdapter = "GPUAdapter";
    static constexpr TName kGPU = "GPU";
    static constexpr TName kSeen = "Seen";
    static constexpr TName kObject = "Object";
  };
  using Ptr = std::shared_ptr<Object>;

  explicit Object(TName name) : class_name(name) {}
  virtual ~Object() = default;

  template <typename T>
  std::shared_ptr<T> SharedSelf() const {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  template <typename T>
  std::shared_ptr<T> SharedSelf() {
    return std::static_pointer_cast<T>(shared_from_this());
  }

  template <typename T>
  std::weak_ptr<T> WeakSelf() const {
    return SharedSelf<T>();
  }

  template <typename T>
  std::weak_ptr<T> WeakSelf() {
    return SharedSelf<T>();
  }

  TName class_name;
};

}  // namespace seen::mod

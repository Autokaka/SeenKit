// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <memory>
#include <string>

namespace seen::mod {

struct Object : public std::enable_shared_from_this<Object> {
  using Ptr = std::shared_ptr<Object>;

  struct Name {
    using Type = const char*;
    static constexpr Type kDrawable = "kDrawable";
    static constexpr Type kFramePacer = "FramePacer";
    static constexpr Type kGPUAdapter = "GPUAdapter";
    static constexpr Type kGPUDevice = "GPUDevice";
    static constexpr Type kGPU = "GPU";
    static constexpr Type kSeen = "Seen";
    static constexpr Type kObject = "Object";
  };

  static bool IsName(const std::string& maybe) {
    return maybe == Name::kDrawable ||    //
           maybe == Name::kFramePacer ||  //
           maybe == Name::kGPUAdapter ||  //
           maybe == Name::kGPU ||         //
           maybe == Name::kSeen ||        //
           maybe == Name::kObject;
  }

  static bool IsName(const char* name) {  //
    return name != nullptr && IsName(std::string(name));
  }

  explicit Object(Name::Type name) : class_name(name) {}
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

  Name::Type class_name;
};

}  // namespace seen::mod

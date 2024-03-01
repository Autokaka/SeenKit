// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <functional>

#include "seen/base/class_ext.h"
#include "seen/base/deferred_task.h"
#include "seen/base/types.h"

namespace seen::rx {

template <typename T>
class Value final {
 public:
  using Token = std::shared_ptr<void>;
  using OnUpdateCallback = std::function<void(const T& new_value)>;

  explicit Value() = default;
  Value(const Value& other) : value_(other.value_) {}
  Value& operator=(const Value& other) {
    if (this != &other) {
      Set(other.value_);
    }
    return *this;
  }
  Value(Value&& other) noexcept : value_(std::move(other.value_)) {}
  Value& operator=(Value&& other) noexcept {
    if (this != &other) {
      Set(std::move(other.value_));
    }
    return *this;
  }
  explicit Value(const T& value) : value_(value) {}
  Value& operator=(const T& new_value) {
    Set(new_value);
    return *this;
  }
  explicit Value(T&& value) noexcept : value_(std::move(value)) {}
  Value& operator=(T&& new_value) noexcept {
    Set(std::move(new_value));
    return *this;
  }

  void Set(const T& new_value) {
    if (value_ == new_value) {
      return;
    }
    DeferredTask apply_new_value([this, &new_value]() { value_ = new_value; });
    if (callbacks_.empty()) {
      return;
    }
    std::vector<OnUpdateCallback> callbacks;
    std::swap(callbacks, callbacks_);
    for (const auto& callback : callbacks) {
      callback(new_value);
    }
    std::swap(callbacks_, callbacks);
  }
  [[nodiscard]] T Get() const { return value_; }

  void OnNext(const OnUpdateCallback& callback) {
    callbacks_.emplace_back(callback);
    callback(value_);
  }

 private:
  std::vector<OnUpdateCallback> callbacks_;
  T value_;
};

template <typename>
struct IsValue : public std::false_type {};

template <typename T>
struct IsValue<Value<T>> : public std::true_type {};

template <typename T>
inline constexpr bool kIsValueV = IsValue<T>::value;

template <typename... V, std::enable_if_t<kIsValueV<V>>...>
inline void Bind(const Closure& callback, V&... values) {
  (values.OnNext([callback](auto) { callback(); }), ...);
}

template <typename T>
class View final {
 public:
  using OnUpdateCallback = typename Value<T>::OnUpdateCallback;

  explicit View(rx::Value<T>* value_ref) : value_ref_(value_ref) {}

  T Get() const { return value_ref_->Get(); }
  void OnNext(const OnUpdateCallback& callback) { value_ref_->OnNext(callback); }

 private:
  rx::Value<T>* value_ref_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(View);
};

}  // namespace seen::rx

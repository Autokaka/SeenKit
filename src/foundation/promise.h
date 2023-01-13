/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/14.
 */

#pragma once

#include <functional>

namespace seen {

template <typename ReturnType>
class CFPromise {
 public:
  using Resolve = std::function<void(ReturnType)>;

  explicit CFPromise(const std::function<void(const Resolve& resolve)>& callback) {
    // TODO(Autokaka): Save calling thead loop.
    // auto event_loop = EventLoop::GetCurrent();
    callback([resolve = resolve_](ReturnType value) {
      if (resolve) {
        // TODO(Autokaka): Resume callback on calling thead.
        resolve(value);
      }
    });
  }

  void Then(const Resolve& resolve) { resolve_ = resolve; }

 private:
  Resolve resolve_;
};

template <>
class CFPromise<void> {
 public:
  using Resolve = std::function<void()>;

  explicit CFPromise(const std::function<void(const Resolve& resolve)>& callback) {
    // TODO(Autokaka): Save calling thead loop.
    // auto event_loop = EventLoop::GetCurrent();
    callback([resolve = resolve_]() {
      if (resolve) {
        // TODO(Autokaka): Resume callback on calling thead.
        resolve();
      }
    });
  }

  void Then(const Resolve& resolve) { resolve_ = resolve; }

 private:
  Resolve resolve_;
};

}  // namespace seen

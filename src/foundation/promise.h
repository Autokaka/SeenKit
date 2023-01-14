/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/14.
 */

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <type_traits>

namespace seen {

enum class CFPromiseState {
  kPending,
  kFulfilled,
  kRejected,
};

template <typename T, typename ReturnType = std::conditional_t<std::is_void_v<T>, nullptr_t, T>>
class CFPromise final {
 public:
  using Resolve = std::function<void(ReturnType)>;
  using Reject = std::function<void(std::string)>;
  using Callback = std::function<void(const Resolve& resolve, const Reject& reject)>;

  explicit CFPromise(const Callback& callback) {
    // TODO(Autokaka): Save calling thead loop.
    // auto event_loop = EventLoop::GetCurrent();

    Resolve resolve = [future = future_](ReturnType value) {
      // TODO(Autokaka): Resume callback on calling thead.
      std::scoped_lock lock(future->mutex);
      future->state = CFPromiseState::kFulfilled;
      future->value = value;
      auto resolve = future->resolve;
      if (resolve) {
        resolve(value);
      }
    };
    Reject reject = [future = future_](const std::string& reason) {
      // TODO(Autokaka): Resume callback on calling thead.
      std::scoped_lock lock(future->mutex);
      future->state = CFPromiseState::kRejected;
      future->reason = reason;
      auto reject = future->reject;
      if (reject) {
        reject(reason);
      }
    };
    callback(resolve, reject);
  }

  CFPromise& Then(const Resolve& resolve) {
    std::scoped_lock lock(future_->mutex);

    if (future_->state == CFPromiseState::kFulfilled) {
      resolve(future_->value);
    } else {
      future_->resolve = resolve;
    }
    return *this;
  }

  CFPromise& Catch(const Reject& reject) {
    std::scoped_lock lock(future_->mutex);

    if (future_->state == CFPromiseState::kRejected) {
      reject(future_->reason);
    } else {
      future_->reject = reject;
    }
    return *this;
  }

 private:
  struct CFFuture : std::enable_shared_from_this<CFFuture> {
    Resolve resolve;
    Reject reject;
    ReturnType value;
    std::string reason;
    CFPromiseState state = CFPromiseState::kPending;
    std::mutex mutex;
  };
  std::shared_ptr<CFFuture> future_;
};

}  // namespace seen

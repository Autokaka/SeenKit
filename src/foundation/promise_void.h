/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/14.
 */

#pragma once

#include "promise.h"

namespace seen {

template <>
class CFPromise<void> {
 public:
  using Resolve = std::function<void()>;
  using Reject = std::function<void(std::string)>;
  using Callback = std::function<void(const Resolve& resolve, const Reject& reject)>;

  explicit CFPromise(const Callback& callback) : future_(std::make_shared<CFFuture>()) {
    // TODO(Autokaka): Save calling thead loop.
    // auto event_loop = EventLoop::GetCurrent();

    Resolve resolve = [future = future_]() {
      // TODO(Autokaka): Resume callback on calling thead.
      std::scoped_lock lock(future->mutex);
      future->state = CFPromiseState::kFulfilled;
      auto resolve = future->resolve;
      if (resolve) {
        resolve();
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
      resolve();
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
    std::string reason;
    CFPromiseState state = CFPromiseState::kPending;
    std::mutex mutex;
  };
  std::shared_ptr<CFFuture> future_;
};

}  // namespace seen

/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/14.
 */

#pragma once

#include "seen/foundation/promise_any.h"

namespace seen {

template <>
class CFPromise<void> {
 public:
  using ResolveCallback = std::function<void()>;
  using Callback = std::function<void(const ResolveCallback& resolve)>;

  static CFPromise Resolve() {
    return CFPromise([](auto resolve) { resolve(); });
  }

  explicit CFPromise(const Callback& callback) : future_(std::make_shared<CFFuture>()) {
    // NOTE(Autokaka): The callback registered in `CFPromise::Then` could be called many times.

    auto looper = CFLooperGetCurrent();

    callback([future = future_, looper]() {
      std::scoped_lock lock(future->mutex);
      future->state = CFPromiseState::kFulfilled;
      looper->DispatchMicro([future]() {
        auto resolve = future->resolve;
        if (resolve) {
          resolve();
        }
      });
    });
  }

  void Then(const ResolveCallback& resolve = nullptr) {
    std::scoped_lock lock(future_->mutex);

    if (future_->state == CFPromiseState::kFulfilled) {
      if (resolve) {
        resolve();
      }
    } else {
      future_->resolve = resolve;
    }
  }

  void Wait(const ResolveCallback& resolve = nullptr) {
    // FIXME(Autokaka): We should consider threading deadlock.
    CFLatch latch;
    Then([resolve, &latch]() {
      if (resolve) {
        resolve();
      }
      latch.Signal();
    });
    latch.Wait();
  }

 private:
  struct CFFuture : std::enable_shared_from_this<CFFuture> {
    ResolveCallback resolve;
    CFPromiseState state = CFPromiseState::kPending;
    std::mutex mutex;
  };
  std::shared_ptr<CFFuture> future_;
};

}  // namespace seen

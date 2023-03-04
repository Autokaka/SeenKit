/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/14.
 */

#pragma once

#include <functional>
#include <memory>
#include <mutex>
#include <string>

#include "seen/foundation/latch.h"
#include "seen/foundation/looper.h"

namespace seen {

enum class CFPromiseState {
  kPending,
  kFulfilled,
};

template <typename ReturnType>
class CFPromise final {
 public:
  using ResolveCallback = std::function<void(ReturnType)>;
  using Callback = std::function<void(const ResolveCallback& resolve)>;

  static CFPromise Resolve(ReturnType value) {
    return CFPromise([&value](auto resolve) { resolve(value); });
  }

  explicit CFPromise(const Callback& callback) : future_(std::make_shared<CFFuture>()) {
    // NOTE(Autokaka): The callback registered in `CFPromise::Then` could be called many times.

    auto looper = CFLooperGetCurrent();

    callback([future = future_, looper](ReturnType value) {
      std::scoped_lock lock(future->mutex);
      future->state = CFPromiseState::kFulfilled;
      future->value = value;
      looper->DispatchMicro([future]() {
        auto resolve = future->resolve;
        if (resolve) {
          resolve(future->value);
        }
      });
    });
  }

  void Then(const ResolveCallback& resolve) {
    std::scoped_lock lock(future_->mutex);

    if (future_->state == CFPromiseState::kFulfilled) {
      if (resolve) {
        resolve(future_->value);
      }
    } else {
      future_->resolve = resolve;
    }
  }

  void Wait(const ResolveCallback& resolve) {
    // FIXME(Autokaka): We should consider threading deadlock.
    CFLatch latch;
    Then([resolve, &latch, future = future_]() {
      if (resolve) {
        resolve(future->value);
      }
      latch.Signal();
    });
    latch.Wait();
  }

 private:
  struct CFFuture : std::enable_shared_from_this<CFFuture> {
    ResolveCallback resolve;
    ReturnType value;
    CFPromiseState state = CFPromiseState::kPending;
    std::mutex mutex;
  };
  std::shared_ptr<CFFuture> future_;
};

}  // namespace seen

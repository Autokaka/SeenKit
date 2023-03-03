/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/21.
 */

#if SEEN_BUILD_DARWIN

#import <Foundation/NSThread.h>

#include <dispatch/dispatch.h>

#include "seen/foundation/looper.h"

namespace seen {

class CFPlatformLooper : public CFLooper {
 public:
  CFPlatformLooper() : main_queue_(dispatch_get_main_queue()) {}

  bool IsCurrentThreadLooper() const override { return [NSThread isMainThread]; }

  void MakeThreadLocalLooper() override {}

  void ConsumeTasks(const Closure& consumer) override {
    TaskContext* context = new TaskContext();
    context->task_consumer = std::move(consumer);
    dispatch_async_f(main_queue_, context, reinterpret_cast<dispatch_function_t>(DoConsumeTasks));
  }

 private:
  struct TaskContext {
    Closure task_consumer;
  };

  static void DoConsumeTasks(TaskContext* context) {
    context->task_consumer();
    delete context;
  }

  dispatch_queue_main_t main_queue_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFPlatformLooper);
};

CFLooperPtr CFGetPlatformLooper() {
  static auto platform_looper = std::make_shared<CFPlatformLooper>();
  return platform_looper;
}

}  // namespace seen

#endif

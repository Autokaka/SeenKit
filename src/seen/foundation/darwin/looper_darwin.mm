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

  void ConsumeMacroTasks(const std::vector<Closure>& macro_tasks) override {
    TaskContext* context = new TaskContext();
    context->looper = this;
    context->tasks = std::move(macro_tasks);
    dispatch_async_f(main_queue_, context, reinterpret_cast<dispatch_function_t>(DoMacroTasks));
  }

 private:
  struct TaskContext {
    CFPlatformLooper* looper;
    std::vector<Closure> tasks;
  };

  dispatch_queue_main_t main_queue_;

  static void DoMacroTasks(TaskContext* context) {
    for (auto macro_task : context->tasks) {
      context->looper->ConsumeMicroTasks();
      macro_task();
    }
    delete context;
  }

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFPlatformLooper);
};

CFLooperPtr CFGetPlatformLooper() {
  static auto platform_looper = std::make_shared<CFPlatformLooper>();
  return platform_looper;
}

}  // namespace seen

#endif

/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/21.
 */

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>

#include "seen/foundation/looper.h"

namespace seen {

CFLooperPtr CFGetPlatformLooper() {
  return std::make_shared<CFPlatformLooper>();
}

class CFPlatformLooper : public CFAbstractLooper {
 public:
  bool IsCurrentThreadLooper() const override { return false; }

  CFPromise<void> DispatchAsync(const Closure& macro_task) override;
  CFPromise<void> DispatchMicro(const Closure& micro_task) override;
};

}  // namespace seen

#endif

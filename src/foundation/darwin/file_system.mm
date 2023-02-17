/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>

#include "foundation/platform.h"

namespace seen::platform {

stdfs::path CFFSGetTempDirectory() {
  return stdfs::path(NSTemporaryDirectory().UTF8String) / "seen";
}

}  // namespace seen::platform

#endif

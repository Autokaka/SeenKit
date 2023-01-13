/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#include "foundation/path.h"

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>

namespace seen::CFPath {

std::string GetSystemTempDirectory() {
  return {NSTemporaryDirectory().UTF8String};
}

}  // namespace seen::CFPath

#endif

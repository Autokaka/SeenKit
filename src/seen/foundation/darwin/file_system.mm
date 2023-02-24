/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#if SEEN_BUILD_DARWIN

#import <Foundation/NSPathUtilities.h>

#include "seen/foundation/file_system.h"

namespace seen::CFFS {

stdfs::path GetTempDirectory() {
  return stdfs::path(NSTemporaryDirectory().UTF8String) / "seen";
}

}  // namespace seen::CFFS

#endif

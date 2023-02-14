/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>

#include <miniunz/unzip.h>
#include <fstream>

#include "foundation/file_system.h"

namespace seen::CFFS {

fs::path GetTempDirectory() {
  return fs::path(NSTemporaryDirectory().UTF8String) / "seen";
}

}  // namespace seen::CFFS

#endif

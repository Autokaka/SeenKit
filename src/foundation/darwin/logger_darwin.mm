/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/16.
 */

#import <Foundation/Foundation.h>

#include "foundation/platform.h"

namespace seen::platform {

void CFLogPrint(const std::string& message) {
  NSLog(@"%@", [NSString stringWithUTF8String:message.c_str()]);
}

}  // namespace seen::platform

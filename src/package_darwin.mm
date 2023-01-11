/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#include <Foundation/Foundation.h>
#import <SeenKit/SeenPackage.h>

#if SEEN_BUILD_DARWIN

#include <memory>
#include <string>

#include "package.h"

@implementation SeenPackage {
  std::unique_ptr<seen::Package> _package;
}

- (instancetype)initWithContentsOfFile:(NSString*)path {
  self = [super init];
  if (self == nil) {
    return nil;
  }

  _package = seen::Package::CreateFromFile(path.UTF8String);
  if (!_package) {
    return nil;
  }
  return self;
}

- (NSDictionary*)info {
  NSMutableDictionary* dict = [NSMutableDictionary dictionary];
  auto cpp_info = _package->GetInfo();
  for (auto& [cpp_key, cpp_value] : cpp_info) {
    [dict setObject:@(cpp_value.data()) forKey:@(cpp_key.data())];
  }
  return [NSDictionary dictionaryWithDictionary:dict];
}

- (NSString*)resourceDirectory {
  return @(_package->GetResourceDirectory().data());
}

- (NSData*)module {
  auto cpp_module = _package->GetModule();
  NSData* data = [NSData dataWithBytes:cpp_module.data() length:cpp_module.size()];
  return data;
}

@end

#endif

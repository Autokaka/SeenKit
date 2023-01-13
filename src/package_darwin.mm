/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#include <Foundation/Foundation.h>
#import <SeenKit/SeenPackage.h>

#if SEEN_BUILD_DARWIN

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

- (NSData*)module {
  auto cpp_module = _package->GetModule();
  NSData* data = [NSData dataWithBytes:cpp_module.data() length:cpp_module.size()];
  return data;
}

- (void)saveFile:(NSData*)data toSandbox:(NSString*)relativePath {
  // clang-format off
  [self saveFile:data toSandbox:relativePath withCompletionHandler:^(BOOL){}];
  // clang-format on
}

- (void)saveFile:(NSData*)data toSandbox:(NSString*)relativePath withCompletionHandler:(void (^)(BOOL))handler {
  std::vector<std::byte> cpp_bytes;
  cpp_bytes.reserve(data.length);
  [data getBytes:cpp_bytes.data() length:data.length];
  _package->SaveFileToSandbox(cpp_bytes, [relativePath UTF8String]).Then(handler);
}

@end

#endif

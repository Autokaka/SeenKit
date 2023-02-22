/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>
#import <SeenKit/SeenPackage.h>

#include "./package.h"

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
  auto cpp_module = _package->GetModuleCopy();
  auto size = cpp_module->GetSize();
  return [NSData dataWithBytesNoCopy:cpp_module->OwnBytes() length:size];
}

- (void)saveFile:(NSData*)data toSandbox:(NSString*)relativePath withCompletionHandler:(void (^)(BOOL))handler {
  std::vector<std::byte> cpp_bytes;
  cpp_bytes.reserve(data.length);
  [data getBytes:cpp_bytes.data() length:data.length];
  _package->SaveFileToSandbox(cpp_bytes, relativePath.UTF8String).Then(handler);
}

- (void)getFileFromSandbox:(NSString*)relativePath withCompletionHandler:(void (^)(NSData*))handler {
  _package->GetFileFromSandbox(relativePath.UTF8String).Then([handler](const seen::CFData::Ptr& cpp_data) {
    auto data_size = cpp_data->GetSize();
    handler([NSData dataWithBytesNoCopy:cpp_data->OwnBytes() length:data_size]);
  });
}

@end

#endif

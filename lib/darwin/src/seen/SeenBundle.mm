// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/07.

#import "seen/SeenBundle+Private.h"

@implementation SeenBundle {
  seen::Bundle::Ptr _cppBundle;
}

- (instancetype)initWithPath:(NSString*)path {
  if (path == nil) {
    return nil;
  }
  self = [super init];
  if (self != nil) {
    _cppBundle = seen::Bundle::CreateFromPath(path.UTF8String);
    if (_cppBundle) {
      return self;
    }
  }
  return nil;
}

- (NSString*)entryPath {
  return @(_cppBundle->GetEntryPath().c_str());
}

- (NSString*)assetsDirectory {
  return @(_cppBundle->GetAssetsDirectory().c_str());
}

@end

@implementation SeenBundle (Private)

- (seen::Bundle::Ptr)cppBundle {
  return _cppBundle;
}

@end

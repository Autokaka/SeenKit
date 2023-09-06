// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#import <SeenKit/SeenEngine.h>

#include "engine.h"

@implementation SeenEngine {
  seen::Engine::Ptr cppEngine_;
}

- (instancetype)initWithBundleData:(NSData*)bundleData {
  self = [super init];
  if (self != nil) {
    auto cppData = seen::CFData::CreateFromBytes((std::byte*)bundleData.bytes, bundleData.length);
    cppEngine_ = std::make_unique<seen::Engine>(cppData);
  }
  return self;
}

@end

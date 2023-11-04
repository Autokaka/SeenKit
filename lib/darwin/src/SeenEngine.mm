// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#import <Metal/Metal.h>
#import <SeenKit/SeenEngine.h>

#import "SeenWeakProxy.h"
#include "engine.h"

@implementation SeenEngine {
  seen::Engine::Ptr _cppEngine;
}

- (seen::Engine*)cppEngine {
  return _cppEngine.get();
}

- (instancetype)initWithLayer:(CAMetalLayer*)layer {
  self = [super init];
  if (self != nil) {
    SeenWeakProxy* layerProxy = [[SeenWeakProxy alloc] initWithId:layer];
    _cppEngine = std::make_unique<seen::Engine>((__bridge_retained void*)layerProxy);
  }
  return self;
}

- (void)updateWithTimeDelta:(NSInteger)timeDeltaMillis completionBlock:(void (^)())onCompleteBlock {
  self.cppEngine->Update(seen::TimeDelta::FromMilliseconds(timeDeltaMillis), onCompleteBlock);
}

@end

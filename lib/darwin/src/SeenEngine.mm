// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#import <SeenKit/SeenEngine.h>

#import "SeenRenderer.h"
#include "engine.h"

@implementation SeenEngine {
  seen::Engine::Ptr _cppEngine;
}

- (instancetype)initWithLayer:(CAMetalLayer*)layer {
  self = [super init];
  if (self != nil) {
    SeenRenderer* renderer = [[SeenRenderer alloc] init];
    renderer.layer = layer;
    _cppEngine = std::make_unique<seen::Engine>((__bridge_retained void*)renderer);
  }
  return self;
}

- (void)updateWithTimeDelta:(NSInteger)timeDeltaMillis completionBlock:(void (^)())onCompleteBlock {
  _cppEngine->Update(seen::TimeDelta::FromMilliseconds(timeDeltaMillis), onCompleteBlock);
}

@end

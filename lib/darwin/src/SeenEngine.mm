// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#import <SeenKit/SeenEngine.h>

#include "engine.h"

@implementation SeenEngine {
  seen::Engine::Ptr _cppEngine;
}

- (seen::Engine*)cppEngine {
  return _cppEngine.get();
}

- (instancetype)init {
  self = [super init];
  if (self != nil) {
    _cppEngine = std::make_unique<seen::Engine>();
  }
  return self;
}

- (void)updateWithTimeDelta:(NSInteger)timeDeltaMillis completionBlock:(void (^)())onCompleteBlock {
  self.cppEngine->Update(seen::TimeDelta::FromMilliseconds(timeDeltaMillis), onCompleteBlock);
}

@end

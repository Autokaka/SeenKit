// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#import <SeenKit/SeenEngine.h>

#import "seen/SeenBundle+Private.h"
#include "seen/engine.h"

@implementation SeenEngine {
  seen::Engine::Ptr _cppEngine;
}

- (instancetype)initWithBundle:(SeenBundle*)bundle {
  self = [super init];
  if (self != nil) {
    _cppEngine = seen::Engine::Create(bundle.cppBundle);
    if (!_cppEngine) {
      return nil;
    }
  }
  return self;
}

@end

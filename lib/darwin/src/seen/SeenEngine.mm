// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/11.

#import <SeenKit/SeenEngine.h>

#import "seen/SeenBundle+Private.h"
#include "seen/engine.h"

@implementation SeenEngine {
  seen::Engine::Ptr _cppEngine;
}

- (instancetype)initWithBundle:(SeenBundle*)bundle {
  if (bundle == nil) {
    return nil;
  }
  self = [super init];
  if (self != nil) {
    _cppEngine = seen::Engine::Create(bundle.cppBundle);
    if (_cppEngine) {
      _bundle = bundle;
      return self;
    }
  }
  return nil;
}

- (void)setMetalLayer:(CAMetalLayer*)metalLayer {
  _cppEngine->Drawable((__bridge void*)metalLayer);
}

- (CAMetalLayer*)metalLayer {
  return (__bridge CAMetalLayer*)_cppEngine->Drawable();
}

- (void)setDrawableSize:(CGSize)size scale:(CGFloat)scale {
  seen::mod::DrawableMetrics metrics{size.width, size.height, scale};
  _cppEngine->DrawableMetrics(metrics);
}

- (void)setPaused:(BOOL)paused {
  _cppEngine->IsRunning(paused == NO);
}

- (BOOL)isPaused {
  auto isPaused = !_cppEngine->IsRunning();
  return static_cast<BOOL>(isPaused);
}

@end

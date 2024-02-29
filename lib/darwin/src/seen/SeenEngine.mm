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

- (void)setView:(NSObject*)view {
  _view = view;
  _cppEngine->SetDrawable((__bridge void*)view);
}

- (void)updateDrawable {
  _cppEngine->UpdateDrawable();
}

- (void)setPaused:(BOOL)paused {
  _cppEngine->IsRunning(paused == NO);
}

- (BOOL)isPaused {
  auto isPaused = !_cppEngine->IsRunning();
  return static_cast<BOOL>(isPaused);
}

@end

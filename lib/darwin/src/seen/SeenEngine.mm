// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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

- (void)setRunning:(BOOL)running {
  _cppEngine->IsRunning(running == YES);
}

- (BOOL)isRunning {
  return static_cast<BOOL>(_cppEngine->IsRunning());
}

@end

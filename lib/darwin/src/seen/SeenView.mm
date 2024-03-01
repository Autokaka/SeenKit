// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <SeenKit/SeenEngine.h>
#import <SeenKit/SeenView.h>

#import "seen/ViewMacros.h"

@interface SeenView ()

SEEN_VIEW_INIT_COMMON_DECL;

@property(nonatomic, strong, nullable) SeenEngine* engine;

@end

@implementation SeenView

SEEN_VIEW_INIT_COMMON_IMPL {
  NSString* path = [NSBundle.mainBundle pathForResource:@"demo" ofType:@"seen"];
  SeenBundle* bundle = [[SeenBundle alloc] initWithPath:path];
  _engine = [[SeenEngine alloc] initWithBundle:bundle];
  _engine.view = self;
}

#if SEEN_BUILD_MACOS

- (void)viewDidMoveToWindow {
  [super viewDidMoveToWindow];
  NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
  [notificationCenter addObserver:self
                         selector:@selector(windowWillClose:)
                             name:NSWindowWillCloseNotification
                           object:self.window];
  [self.engine updateDrawable];
  self.engine.running = YES;
}

- (void)windowWillClose:(NSNotification*)notification {
  if (notification.object == self.window) {
    self.engine = nil;
  }
}

#pragma mark - Resize
- (void)viewDidChangeBackingProperties {
  [super viewDidChangeBackingProperties];
  [self.engine updateDrawable];
}

- (void)setFrameSize:(NSSize)size {
  [super setFrameSize:size];
  [self.engine updateDrawable];
}

- (void)setBoundsSize:(NSSize)size {
  [super setBoundsSize:size];
  [self.engine updateDrawable];
}

#endif  // SEEN_BUILD_MACOS

@end

// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#if SEEN_BUILD_MACOS

#import "seen/SeenBaseView+Private.h"

@implementation SeenNSView

- (void)viewDidMoveToWindow {
  [super viewDidMoveToWindow];
  NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
  [notificationCenter addObserver:self
                         selector:@selector(windowWillClose:)
                             name:NSWindowWillCloseNotification
                           object:self.window];
  self.paused = NO;
  [self.engine updateDrawable];
}

- (void)windowWillClose:(NSNotification*)notification {
  if (notification.object == self.window) {
    self.paused = YES;
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

@end

#endif

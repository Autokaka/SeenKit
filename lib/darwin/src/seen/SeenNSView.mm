// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#if SEEN_BUILD_MACOS

#import <CoreVideo/CVDisplayLink.h>
#import <dispatch/dispatch.h>

#import "seen/SeenBaseView+Private.h"

@implementation SeenNSView

- (void)initCommon {
  self.wantsLayer = YES;
  self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;

  NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
  // Register to be notified when the window closes so that you
  // can stop the display link
  [notificationCenter addObserver:self
                         selector:@selector(windowWillClose:)
                             name:NSWindowWillCloseNotification
                           object:self.window];

  [super initCommon];
}

- (CALayer*)makeBackingLayer {
  return [CAMetalLayer layer];
}

- (void)viewDidMoveToWindow {
  [super viewDidMoveToWindow];
  [self resizeDrawable:self.window.screen.backingScaleFactor];
  self.paused = NO;
}

- (void)windowWillClose:(NSNotification*)notification {
  // Stop the display link when the window is closing since there
  // is no point in drawing something that can't be seen
  if (notification.object == self.window) {
    self.paused = YES;
  }
}

#pragma mark - Resize

- (void)viewDidChangeBackingProperties {
  [super viewDidChangeBackingProperties];
  [self resizeDrawable:self.window.screen.backingScaleFactor];
}

- (void)setFrameSize:(NSSize)size {
  [super setFrameSize:size];
  [self resizeDrawable:self.window.screen.backingScaleFactor];
}

- (void)setBoundsSize:(NSSize)size {
  [super setBoundsSize:size];
  [self resizeDrawable:self.window.screen.backingScaleFactor];
}

@end

#endif

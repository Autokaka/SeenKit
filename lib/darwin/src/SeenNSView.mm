// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#if SEEN_BUILD_MACOS

#import "SeenBaseView+Private.h"

#import <CoreVideo/CVDisplayLink.h>
#import <dispatch/dispatch.h>

@implementation SeenNSView {
  CVDisplayLinkRef _displayLink;
  dispatch_source_t _displaySource;
}

- (void)initCommon {
  self.wantsLayer = YES;
  self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;
  [super initCommon];
}

- (CALayer*)makeBackingLayer {
  return [CAMetalLayer layer];
}

- (void)setPaused:(BOOL)paused {
  super.paused = paused;
  [self setPausedInternal:paused];
}

- (void)dealloc {
  [self safeReleaseCVDisplayLink];
}

- (void)viewDidMoveToWindow {
  [super viewDidMoveToWindow];
  [self safeReleaseCVDisplayLink];
  [self setupCVDisplayLinkForScreen:self.window.screen];
  [self resizeDrawable:self.window.screen.backingScaleFactor];
}

#pragma mark - Vsync

- (void)setPausedInternal:(BOOL)paused {
  if (_displayLink == nil) {
    return;
  }
  if (paused == YES) {
    CVDisplayLinkStop(_displayLink);
  } else {
    CVDisplayLinkStart(_displayLink);
  }
}

- (BOOL)isPausedInternal {
  if (_displayLink == nil) {
    return YES;
  }
  if (CVDisplayLinkIsRunning(_displayLink) == NO) {
    return YES;
  }
  return self.isPaused;
}

- (void)safeReleaseCVDisplayLink {
  // Stop the display link BEFORE releasing anything in the view otherwise the display link
  // thread may call into the view and crash when it encounters something that no longer
  // exists
  [self setPausedInternal:YES];
  if (_displayLink != nil) {
    CVDisplayLinkRelease(_displayLink);
    dispatch_source_cancel(_displaySource);
  }
}

- (void)windowWillClose:(NSNotification*)notification {
  // Stop the display link when the window is closing since there
  // is no point in drawing something that can't be seen
  if (notification.object == self.window) {
    [self safeReleaseCVDisplayLink];
  }
}

static CVReturn DispatchRenderLoop(CVDisplayLinkRef displayLink,
                                   const CVTimeStamp* now,
                                   const CVTimeStamp* outputTime,
                                   CVOptionFlags flagsIn,
                                   CVOptionFlags* flagsOut,
                                   void* displayLinkContext) {
  // 'DispatchRenderLoop' is always called on a secondary thread.  Merge the dispatch source
  // setup for the main queue so that rendering occurs on the main thread
  __weak dispatch_source_t source = (__bridge dispatch_source_t)displayLinkContext;
  dispatch_source_merge_data(source, 1);
  return kCVReturnSuccess;
}

- (BOOL)setupCVDisplayLinkForScreen:(NSScreen*)screen {
  // The CVDisplayLink callback, DispatchRenderLoop, never executes
  // on the main thread. To execute rendering on the main thread, create
  // a dispatch source using the main queue (the main thread).
  // DispatchRenderLoop merges this dispatch source in each call
  // to execute rendering on the main thread.
  _displaySource = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
  __weak SeenNSView* weakSelf = self;
  dispatch_source_set_event_handler(_displaySource, ^() {
    @autoreleasepool {
      SeenNSView* strongSelf = weakSelf;
      if ([strongSelf isPausedInternal] == NO) {
        [strongSelf setPausedInternal:YES];
        [strongSelf.engine updateWithTimeDelta:0
                               completionBlock:^{
                                 SeenNSView* strongSelf = weakSelf;
                                 if (strongSelf.isPaused == NO) {
                                   [strongSelf setPausedInternal:NO];
                                 }
                               }];
      }
    }
  });
  dispatch_resume(_displaySource);

  // Create a display link capable of being used with all active displays
  CVReturn cvReturn = CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
  if (cvReturn != kCVReturnSuccess) {
    return NO;
  }

  // Set DispatchRenderLoop as the callback function and
  // supply _displaySource as the argument to the callback.
  cvReturn = CVDisplayLinkSetOutputCallback(_displayLink, &DispatchRenderLoop, (__bridge void*)_displaySource);
  if (cvReturn != kCVReturnSuccess) {
    return NO;
  }

  // Associate the display link with the display on which the
  // view resides
  CGDirectDisplayID viewDisplayID =
      (CGDirectDisplayID)[self.window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue];
  cvReturn = CVDisplayLinkSetCurrentCGDisplay(_displayLink, viewDisplayID);
  if (cvReturn != kCVReturnSuccess) {
    return NO;
  }

  [self setPausedInternal:self.isPaused];

  NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
  // Register to be notified when the window closes so that you
  // can stop the display link
  [notificationCenter addObserver:self
                         selector:@selector(windowWillClose:)
                             name:NSWindowWillCloseNotification
                           object:self.window];
  return YES;
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

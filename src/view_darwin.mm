/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#import <SeenKit/SeenView.h>

#include "seen/foundation/logger.h"

namespace {

CVReturn DispatchRenderLoop(CVDisplayLinkRef displayLink,
                            const CVTimeStamp* now,
                            const CVTimeStamp* outputTime,
                            CVOptionFlags flagsIn,
                            CVOptionFlags* flagsOut,
                            void* displayLinkContext) {
  // 'DispatchRenderLoop' is always called on a secondary thread. Merge the dispatch source
  // setup for the main queue so that rendering occurs on the main thread.
  __weak dispatch_source_t source = (__bridge dispatch_source_t)displayLinkContext;
  dispatch_source_merge_data(source, 1);
  return kCVReturnSuccess;
}

constexpr auto kSeenViewRefreshRate = 60;

}  // namespace

@implementation SeenView {
  SeenEngine* _engine;
#if SEEN_BUILD_IOS
#else
  CVDisplayLinkRef _displayLink;
  dispatch_source_t _displaySource;
#endif
}

- (instancetype)init {
  self = [super init];
  _engine = [[SeenEngine alloc] init];
  // In AppKit, you make the view layer backed by setting the view's wantsLayer property.
  // This triggers a call to the view's  makeBackingLayer method, which returns a CAMetalLayer object.
  self.wantsLayer = YES;
  self.layerContentsRedrawPolicy = NSViewLayerContentsRedrawDuringViewResize;

  NSNotificationCenter* notificationCenter = [NSNotificationCenter defaultCenter];
  [notificationCenter addObserver:self
                         selector:@selector(windowWillClose:)
                             name:NSWindowWillCloseNotification
                           object:self.window];
  return self;
}

- (BOOL)isPaused {
  if (_displaySource == nil || _displayLink == nil) {
    return YES;
  }
  return static_cast<BOOL>(!CVDisplayLinkIsRunning(_displayLink));
}

- (void)setPaused:(BOOL)paused {
  if (_displaySource == nil || _displayLink == nil) {
    return;
  }

  if (self.isPaused == paused) {
    return;
  }

  if (paused == YES) {
    CVDisplayLinkStop(_displayLink);
    dispatch_source_cancel(_displaySource);
  } else {
    dispatch_resume(_displaySource);
    CVDisplayLinkStart(_displayLink);
  }
}

- (void)runPackage:(SeenPackage*)package {
  return [_engine runPackage:package];
}

- (void)runPackage:(SeenPackage*)package withCompletionHandler:(void (^)(BOOL))handler {
  return [_engine runPackage:package withCompletionHandler:handler];
}

- (void)reset {
  self.paused = YES;
  [_engine reset];
}

- (void)resetWithCompletionHandler:(void (^)(void))handler {
  self.paused = YES;
  [_engine resetWithCompletionHandler:handler];
}

- (CAMetalLayer*)metalLayer {
  return (CAMetalLayer*)self.layer;
}

- (void)drawFrame {
  [_engine draw:1.0F / kSeenViewRefreshRate];
}

- (void)resizeDrawable:(CGFloat)scaleFactor {
  CGSize viewSize = self.bounds.size;
  viewSize.width *= scaleFactor;
  viewSize.height *= scaleFactor;
  if (viewSize.width <= 0 || viewSize.height <= 0) {
    return;
  }

  CGSize drawableSize = self.metalLayer.drawableSize;
  if (CGSizeEqualToSize(drawableSize, viewSize)) {
    return;
  }

  self.metalLayer.drawableSize = viewSize;
}

#if SEEN_BUILD_IOS
#else
- (CALayer*)makeBackingLayer {
  return [CAMetalLayer layer];
}

- (void)viewDidMoveToWindow {
  [super viewDidMoveToWindow];
  [self initDisplayLinkIfNeeded];
  [self resizeDrawable:self.window.screen.backingScaleFactor];
  self.paused = static_cast<BOOL>(self.window == nil);
}

- (void)windowWillClose:(NSNotification*)notification {
  if (notification.object == self.window) {
    self.paused = YES;
  }
}

- (void)dealloc {
  [self reset];
  CVDisplayLinkRelease(_displayLink);
}

- (void)initDisplayLinkIfNeeded {
  if (_displaySource != nil && _displayLink != nil) {
    return;
  }

  // The CVDisplayLink callback, DispatchRenderLoop, never executes
  // on the main thread. To execute rendering on the main thread, create
  // a dispatch source using the main queue (the main thread).
  // DispatchRenderLoop merges this dispatch source in each call
  // to execute rendering on the main thread.
  _displaySource = dispatch_source_create(DISPATCH_SOURCE_TYPE_DATA_ADD, 0, 0, dispatch_get_main_queue());
  __weak SeenView* weakSelf = self;
  dispatch_source_set_event_handler(_displaySource, ^() {
    @autoreleasepool {
      [weakSelf drawFrame];
    }
  });
  CVReturn cvReturn = CVDisplayLinkCreateWithActiveCGDisplays(&_displayLink);
  SEEN_ASSERT(cvReturn == kCVReturnSuccess);
  cvReturn = CVDisplayLinkSetOutputCallback(_displayLink, &DispatchRenderLoop, (__bridge void*)_displaySource);
  SEEN_ASSERT(cvReturn == kCVReturnSuccess);
  CGDirectDisplayID viewDisplayID = [self.window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntValue];
  cvReturn = CVDisplayLinkSetCurrentCGDisplay(_displayLink, viewDisplayID);
  SEEN_ASSERT(cvReturn == kCVReturnSuccess);
}

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
#endif

@end

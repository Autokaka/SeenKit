#import <Metal/Metal.h>

#import "seen/SeenBaseView+Private.h"
#import "seen/ViewMacros.h"

@implementation SeenBaseView

- (CAMetalLayer*)metalLayer {
  return (CAMetalLayer*)self.layer;
}

SEEN_VIEW_INIT_COMMON_IMPL {
  self.metalLayer.device = MTLCreateSystemDefaultDevice();
  self.metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm_sRGB;
  NSString* path = [NSBundle.mainBundle pathForResource:@"demo" ofType:@"seen"];
  SeenBundle* bundle = [[SeenBundle alloc] initWithPath:path];
  _engine = [[SeenEngine alloc] initWithBundle:bundle];
}

- (void)resizeDrawable:(CGFloat)scaleFactor {
  CGSize newSize = self.bounds.size;
  newSize.width *= scaleFactor;
  newSize.height *= scaleFactor;
  newSize.width = MAX(0, newSize.width);
  newSize.height = MAX(0, newSize.height);
  self.metalLayer.contentsScale = scaleFactor;
  self.metalLayer.drawableSize = newSize;
}

@end

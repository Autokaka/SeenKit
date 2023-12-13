#import <Metal/Metal.h>

#import "SeenBaseView+Private.h"
#import "ViewMacros.h"

@implementation SeenBaseView

- (CAMetalLayer*)metalLayer {
  return (CAMetalLayer*)self.layer;
}

SEEN_VIEW_INIT_COMMON_IMPL {
  _engine = [[SeenEngine alloc] initWithLayer:self.metalLayer];
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

#import "SeenBaseView+Private.h"
#import "ViewMacros.h"

@implementation SeenBaseView

- (CAMetalLayer*)metalLayer {
  return (CAMetalLayer*)self.layer;
}

SEEN_VIEW_INIT_COMMON_IMPL {
  _engine = [[SeenEngine alloc] init];
}

- (void)resizeDrawable:(CGFloat)scaleFactor {
  CGSize newSize = self.bounds.size;
  newSize.width *= scaleFactor;
  newSize.height *= scaleFactor;
  if (newSize.width <= 0 || newSize.width <= 0) {
    return;
  }
  if (CGSizeEqualToSize(newSize, self.metalLayer.drawableSize)) {
    return;
  }
  self.metalLayer.drawableSize = newSize;
}

@end

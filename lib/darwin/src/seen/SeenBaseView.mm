#import <Metal/Metal.h>

#import "seen/SeenBaseView+Private.h"
#import "seen/ViewMacros.h"

@implementation SeenBaseView

SEEN_VIEW_INIT_COMMON_IMPL {
  NSString* path = [NSBundle.mainBundle pathForResource:@"demo" ofType:@"seen"];
  SeenBundle* bundle = [[SeenBundle alloc] initWithPath:path];
  _engine = [[SeenEngine alloc] initWithBundle:bundle];
  _engine.view = self;
}

- (void)setPaused:(BOOL)paused {
  [self.engine setPaused:paused];
}

@end

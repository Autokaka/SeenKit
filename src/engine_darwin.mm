/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#import <SeenKit/SeenKit.h>

#include <memory>

#include "seen_kit.h"

@implementation SEENEngine {
  SEEN_OS_VIEW* _view;
  std::unique_ptr<seen::Engine> _engine;
}

- (SEEN_OS_VIEW*)view {
  if (_view == nil) {
    _view = [[SEEN_OS_VIEW alloc] init];
  }
  return _view;
}

- (void)runPackage:(SEENPackage*)package {
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec {
  // clang-format off
  [self draw:timeDeltaMillisec withCompletionHandler:^(void){}];
  // clang-format on
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(nonnull void (^)(void))handler {
}

@end

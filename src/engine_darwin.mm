/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#import <SeenKit/SeenEngine.h>

#include <memory>

#include "engine.h"

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
  // clang-format off
  [self runPackage:package withCompletionHandler:^(void){}];
  // clang-format on
}

- (void)runPackage:(SEENPackage*)package withCompletionHandler:(void (^)())handler {
  NSUInteger byteLength = package.module.length;
  std::vector<std::byte> bytes;
  bytes.reserve(byteLength);
  [package.module getBytes:bytes.data() length:byteLength];
  _engine->RunModule(bytes, handler);
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec {
  _engine->Draw(timeDeltaMillisec);
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(void (^)(void))handler {
  _engine->Draw(timeDeltaMillisec, handler);
}

- (void)reset {
  _engine->Reset();
}

- (void)resetWithCompletionHandler:(void (^)())handler {
  _engine->Reset(handler);
}

@end

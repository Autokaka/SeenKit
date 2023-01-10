/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#import <SeenKit/SeenKit.h>
#include <objc/NSObjCRuntime.h>

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

- (BOOL)paused {
  return (BOOL)_engine->IsPaused();
}

- (void)play {
  _engine->Play();
}

- (void)playWithCompletionHandler:(void (^)(void))handler {
  _engine->Play(handler);
}

- (void)pause {
  _engine->Pause();
}

- (void)pauseWithCompletionHandler:(void (^)(void))handler {
  _engine->Pause(handler);
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

@end

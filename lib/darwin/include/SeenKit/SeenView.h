// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#import "SeenMacros.h"

#if SEEN_BUILD_MACOS
#import <AppKit/AppKit.h>
#define SEEN_OS_VIEW NSView
#else
#import <UIKit/UIKit.h>
#define SEEN_OS_VIEW UIView
#endif

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenBaseView : SEEN_OS_VIEW

@property(nonatomic, getter=isPaused) BOOL paused;

@end

#if SEEN_BUILD_MACOS
SEEN_EXPORT @interface SeenNSView : SeenBaseView
@end
#define SeenView SeenNSView
#else
SEEN_EXPORT @interface SeenUIView : SeenBaseView
@end
#define SeenView SeenUIView
#endif  // SEEN_BUILD_MACOS

NS_ASSUME_NONNULL_END

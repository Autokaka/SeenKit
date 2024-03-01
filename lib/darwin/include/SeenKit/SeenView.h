// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#import <SeenKit/SeenMacros.h>

#if SEEN_BUILD_MACOS
#import <AppKit/AppKit.h>
#define SEEN_OS_VIEW NSView
#define SEEN_OS_VIEW_REPRESENTABLE NSViewRepresentable
#else
#import <UIKit/UIKit.h>
#define SEEN_OS_VIEW UIView
#define SEEN_OS_VIEW_REPRESENTABLE UIViewRepresentable
#endif

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenView : SEEN_OS_VIEW

@end

NS_ASSUME_NONNULL_END

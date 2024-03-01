// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import "SeenMacros.h"

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

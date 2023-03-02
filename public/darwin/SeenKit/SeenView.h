/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include "SeenBuildConfig.h"

#import <QuartzCore/CAMetalLayer.h>
#import <SeenKit/SeenEngine.h>
#if SEEN_BUILD_IOS
#import <UIKit/UIKit.h>
#else
#import <AppKit/AppKit.h>
#endif

SEEN_EXPORT @interface SeenView : SEEN_OS_VIEW<CALayerDelegate>

@property(nonatomic, getter=isPaused) BOOL paused;

- (void)runPackage:(nullable SeenPackage*)package;
- (void)runPackage:(nullable SeenPackage*)package withCompletionHandler:(nonnull void (^)(BOOL))handler;

- (void)reset;
- (void)resetWithCompletionHandler:(nonnull void (^)(void))handler;

@end

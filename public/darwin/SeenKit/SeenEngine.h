/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>
#import <Foundation/NSObject.h>

#import "SeenPackage.h"

#if SEEN_BUILD_IOS
#import <UIKit/UIKit.h>
#define SEEN_OS_VIEW UIView
#else
#import <Appkit/Appkit.h>
#define SEEN_OS_VIEW NSView
#endif

@interface SEENEngine : NSObject

- (void)runPackage:(nullable SEENPackage*)package;
- (void)runPackage:(nullable SEENPackage*)package withCompletionHandler:(nonnull void (^)(void))handler;

- (void)draw:(NSTimeInterval)timeDeltaMillisec;
- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(nonnull void (^)(void))handler;

- (void)reset;
- (void)resetWithCompletionHandler:(nonnull void (^)(void))handler;

@end

#endif

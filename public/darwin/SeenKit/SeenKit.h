/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <Foundation/Foundation.h>
#include <Foundation/NSObject.h>

#if SEEN_BUILD_DARWIN

#if SEEN_BUILD_IOS
#import <UIKit/UIKit.h>
#define SEEN_OS_VIEW UIView
#else
#include <Appkit/Appkit.h>
#define SEEN_OS_VIEW NSView
#endif

@interface SEENPackage : NSObject

@property(readonly, copy, nullable, nonatomic) NSDictionary* info;

@property(readonly, copy, nonnull, nonatomic) NSString* resourceDirectory;

@property(readonly, copy, nonnull, nonatomic) NSData* module;

- (nullable instancetype)initWithContentsOfFile:(nullable NSString*)path;

@end

@interface SEENEngine : NSObject

@property(readonly, nonnull, nonatomic) SEEN_OS_VIEW* view;
@property(readonly, nonatomic) BOOL isPaused;

- (void)runPackage:(nullable SEENPackage*)package;
- (void)runPackage:(nullable SEENPackage*)package withCompletionHandler:(nonnull void (^)(void))handler;

- (void)play;
- (void)playWithCompletionHandler:(nonnull void (^)(void))handler;
- (void)pause;
- (void)pauseWithCompletionHandler:(nonnull void (^)(void))handler;

- (void)draw:(NSTimeInterval)timeDeltaMillisec;
- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(nonnull void (^)(void))handler;

- (void)reset;
- (void)resetWithCompletionHandler:(nonnull void (^)(void))handler;

@end

#endif

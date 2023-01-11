/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>
#import <Metal/Metal.h>

#import "SeenPackage.h"

@interface SeenEngine : NSObject

@property(atomic) CGSize drawableSize;
@property(nonnull, readonly, atomic, strong) id<MTLTexture> colorTexture;
@property(nonnull, readonly, atomic, strong) id<MTLTexture> depthTexture;

- (void)runPackage:(nullable SeenPackage*)package;
- (void)runPackage:(nullable SeenPackage*)package withCompletionHandler:(nonnull void (^)(void))handler;

- (void)draw:(NSTimeInterval)timeDeltaMillisec;
- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(nonnull void (^)(void))handler;

- (void)reset;
- (void)resetWithCompletionHandler:(nonnull void (^)(void))handler;

@end

#endif

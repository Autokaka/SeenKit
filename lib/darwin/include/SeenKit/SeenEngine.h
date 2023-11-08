// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>

#import "SeenMacros.h"

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenEngine : NSObject

- (instancetype)initWithLayer:(CAMetalLayer*)layer NS_DESIGNATED_INITIALIZER;

- (void)updateWithTimeDelta:(NSInteger)timeDeltaMillis completionBlock:(nullable void (^)(void))onCompleteBlock;

@end

NS_ASSUME_NONNULL_END

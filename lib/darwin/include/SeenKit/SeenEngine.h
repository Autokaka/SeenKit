// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#import <Foundation/Foundation.h>

#import "SeenMacros.h"

SEEN_EXPORT @interface SeenEngine : NSObject

- (void)updateWithTimeDelta:(NSInteger)timeDeltaMillis completionBlock:(nullable void (^)(void))onCompleteBlock;

@end

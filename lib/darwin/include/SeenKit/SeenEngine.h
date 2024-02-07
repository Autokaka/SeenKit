// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#import <Foundation/Foundation.h>
#import <SeenKit/SeenBundle.h>
#import <SeenKit/SeenMacros.h>

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenEngine : NSObject

- (nullable instancetype)initWithBundle:(SeenBundle*)bundle NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END

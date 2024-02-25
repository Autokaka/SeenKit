// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>
#import <SeenKit/SeenBundle.h>
#import <SeenKit/SeenMacros.h>

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenEngine : NSObject

@property(atomic, strong, nullable) CAMetalLayer* metalLayer;
@property(nonatomic, assign, getter=isPaused) BOOL paused;
@property(nonatomic, strong, readonly) SeenBundle* bundle;

- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithBundle:(nullable SeenBundle*)bundle NS_DESIGNATED_INITIALIZER;

- (void)setDrawableSize:(CGSize)size scale:(CGFloat)scale;

@end

NS_ASSUME_NONNULL_END

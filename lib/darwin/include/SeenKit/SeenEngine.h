// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>

#import "SeenBundle.h"
#import "SeenMacros.h"

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenEngine : NSObject

@property(nonatomic, weak, nullable) NSObject* view;
@property(nonatomic, assign, getter=isRunning) BOOL running;
@property(nonatomic, strong, readonly) SeenBundle* bundle;

- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithBundle:(nullable SeenBundle*)bundle NS_DESIGNATED_INITIALIZER;

- (void)updateDrawable;

@end

NS_ASSUME_NONNULL_END

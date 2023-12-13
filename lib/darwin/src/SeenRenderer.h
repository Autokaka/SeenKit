// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/11.

#import <QuartzCore/CAMetalLayer.h>

NS_ASSUME_NONNULL_BEGIN

@interface SeenRenderer : NSObject

@property(nonatomic, weak) CAMetalLayer* layer;

- (nullable CAMetalLayer*)lockLayer;

// TODO(Autokaka): Render scene...
// TODO(Autokaka): Render node...

@end

NS_ASSUME_NONNULL_END

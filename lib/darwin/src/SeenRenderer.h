// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/11.

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

NS_ASSUME_NONNULL_BEGIN

@interface SeenRenderer : NSObject

@property(nonatomic, weak, readonly) CAMetalLayer* layer;
@property(nonatomic, assign) CGSize drawableSize;

- (instancetype)initWithLayer:(CAMetalLayer*)layer NS_DESIGNATED_INITIALIZER;

- (void)drawScene:(MTLClearColor)backgroundColor;

- (void)drawNode:(const void*)data length:(NSUInteger)length;

- (void)presentDrawable;

@end

NS_ASSUME_NONNULL_END

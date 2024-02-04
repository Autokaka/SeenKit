// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/11.

#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

#include <simd/simd.h>
#include <vector>

struct DefaultVertexInput {
  vector_float2 position;
};

struct DefaultUniformInput {
  vector_float3 color;
};

NS_ASSUME_NONNULL_BEGIN

@interface SeenRenderer : NSObject

@property(nonatomic, weak, readonly) CAMetalLayer* layer;
@property(nonatomic, assign) CGSize drawableSize;

- (instancetype)initWithLayer:(CAMetalLayer*)layer NS_DESIGNATED_INITIALIZER;

- (void)drawScene:(MTLClearColor)backgroundColor;

- (void)drawSpriteNode:(std::vector<DefaultVertexInput>)vertices uniform:(const DefaultUniformInput&)uniform;

- (void)presentDrawable;

@end

NS_ASSUME_NONNULL_END

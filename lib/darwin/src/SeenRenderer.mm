// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/11.

#include <simd/simd.h>

#import "SeenRenderer.h"

@interface SeenRenderer ()

@property(nonatomic, strong, readonly) id<MTLDevice> device;
@property(nonatomic, assign, readonly) MTLPixelFormat drawablePixelFormat;
@property(nonatomic, strong, readonly) id<MTLCommandQueue> commandQueue;
@property(nonatomic, strong, readonly) MTLRenderPassDescriptor* sceneDescriptor;
@property(nonatomic, strong) id<MTLRenderPipelineState> defaultPipelineState;

@property(nonatomic, strong, nullable) id<MTLCommandBuffer> commandBuffer;
@property(nonatomic, strong, nullable) id<CAMetalDrawable> drawable;

@end

@implementation SeenRenderer

- (instancetype)initWithLayer:(CAMetalLayer*)layer {
  self = [super init];
  if (self != nil) {
    _layer = layer;
    _device = layer.device;
    _drawablePixelFormat = layer.pixelFormat;
    _commandQueue = [layer.device newCommandQueue];
    _sceneDescriptor = [[MTLRenderPassDescriptor alloc] init];
  }
  return self;
}

- (CGSize)drawableSize {
  return self.layer.drawableSize;
}

- (id<MTLRenderPipelineState>)defaultPipelineState {
  if (_defaultPipelineState == nil) {
    const char* shaderSource = R"__(
#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

struct DefaultVertexInput {
  vector_float2 position;
};

struct DefaultUniformInput {
  vector_float3 color;
};

struct DefaultVertexOutput {
  float4 position [[position]];
  float3 color;
};

vertex DefaultVertexOutput vertexFunction(uint vertexID [[vertex_id]],
                                 constant DefaultVertexInput* vertices [[buffer(0)]],
                                 constant DefaultUniformInput& uniform [[buffer(1)]]) {
    DefaultVertexOutput out;
    out.position.xy = vertices[vertexID].position.xy;
    out.position.z = 0.0;
    out.position.w = 1.0;
    out.color = uniform.color;
    return out;
}

fragment float4 fragmentFunction(DefaultVertexOutput in [[stage_in]]) {
    return float4(in.color, 1.0);
}
)__";
    NSError* error = nil;
    auto defaultLibrary = [_device newLibraryWithSource:[NSString stringWithUTF8String:shaderSource]
                                                options:nil
                                                  error:&error];
    auto vertexFunction = [defaultLibrary newFunctionWithName:@"vertexFunction"];
    auto fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentFunction"];
    MTLRenderPipelineDescriptor* pipelineDescriptor = [[MTLRenderPipelineDescriptor alloc] init];
    pipelineDescriptor.label = @"DefaultPipeline";
    pipelineDescriptor.vertexFunction = vertexFunction;
    pipelineDescriptor.fragmentFunction = fragmentFunction;
    pipelineDescriptor.colorAttachments[0].pixelFormat = _drawablePixelFormat;
    _defaultPipelineState = [_device newRenderPipelineStateWithDescriptor:pipelineDescriptor error:nil];
  }
  return _defaultPipelineState;
}

- (void)drawScene:(MTLClearColor)backgroundColor {
  self.sceneDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
  self.sceneDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
  self.sceneDescriptor.colorAttachments[0].clearColor = backgroundColor;
  self.commandBuffer = [self.commandQueue commandBuffer];
  self.drawable = [self.layer nextDrawable];
  self.sceneDescriptor.colorAttachments[0].texture = self.drawable.texture;
}

- (void)drawSpriteNode:(std::vector<DefaultVertexInput>)vertices uniform:(const DefaultUniformInput&)uniform {
  auto renderEncoder = [self.commandBuffer renderCommandEncoderWithDescriptor:self.sceneDescriptor];
  [renderEncoder setRenderPipelineState:self.defaultPipelineState];
  auto vbo = [self.device newBufferWithBytes:vertices.data()
                                      length:sizeof(vertices[0]) * vertices.size()
                                     options:MTLResourceStorageModeShared];
  [renderEncoder setVertexBuffer:vbo offset:0 atIndex:0];
  [renderEncoder setVertexBytes:&uniform length:sizeof(uniform) atIndex:1];
  [renderEncoder drawPrimitives:MTLPrimitiveTypeTriangle vertexStart:0 vertexCount:vertices.size()];
  [renderEncoder endEncoding];
}

- (void)presentDrawable {
  [self.commandBuffer presentDrawable:self.drawable];
  [self.commandBuffer commit];
}

@end

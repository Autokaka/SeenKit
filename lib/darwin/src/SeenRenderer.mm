// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/11.

#import "SeenRenderer.h"

@interface SeenRenderer ()

@property(nonatomic, strong) id<MTLDevice> device;
@property(nonatomic, assign) MTLPixelFormat drawablePixelFormat;
@property(nonatomic, strong) id<MTLCommandQueue> commandQueue;
@property(nonatomic, strong) MTLRenderPassDescriptor* sceneDescriptor;
@property(nonatomic, strong) id<MTLRenderPipelineState> defaultPipelineState;

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

struct UniformInput {
  vector_uint2 viewportSize;
  float scale;
};

struct VertexInput {
  vector_float2 position;
  vector_float3 color;
};

struct VertexOutput {
  float4 clipSpacePosition [[position]];
  float3 color;
};

vertex VertexOutput vertexFunction(uint vertexID [[vertex_id]],
                                 constant VertexInput* vertices [[buffer(0)]],
                                 constant UniformInput& uniforms [[buffer(1)]]) {
    VertexOutput out;
    float2 pixelSpacePosition = vertexArray[vertexID].position.xy;
    pixelSpacePosition *= uniforms.scale;
    float2 viewportSize = float2(uniforms.viewportSize);
    out.clipSpacePosition.xy = pixelSpacePosition / (viewportSize / 2.0);
    out.clipSpacePosition.z = 0.0;
    out.clipSpacePosition.w = 1.0;
    out.color = vertexArray[vertexID].color;
    return out;
}

fragment float4 fragmentFunction(VertexOutput in [[stage_in]]) {
    return float4(in.color, 1.0);
}
)__";
    id<MTLLibrary> defaultLibrary = [_device newLibraryWithSource:[NSString stringWithUTF8String:shaderSource]
                                                          options:nil
                                                            error:nil];
    id<MTLFunction> vertexFunction = [defaultLibrary newFunctionWithName:@"vertexFunction"];
    id<MTLFunction> fragmentFunction = [defaultLibrary newFunctionWithName:@"fragmentFunction"];
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
  _sceneDescriptor.colorAttachments[0].loadAction = MTLLoadActionClear;
  _sceneDescriptor.colorAttachments[0].storeAction = MTLStoreActionStore;
  _sceneDescriptor.colorAttachments[0].clearColor = backgroundColor;
}

@end

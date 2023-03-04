/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#if SEEN_BUILD_DARWIN

#import <SeenKit/SeenEngine.h>

#include <vector>

#include "engine.h"

@implementation SeenEngine {
  std::unique_ptr<seen::Engine> _engine;

  CGSize _drawableSize;

  id<MTLDevice> _device;
  id<MTLTexture> _colorTexture;
  id<MTLTexture> _depthTexture;
  MTLTextureDescriptor* _textureDescriptor;
}

- (id<MTLDevice>)device {
  if (_device == nil) {
    _device = MTLCreateSystemDefaultDevice();
  }
  return _device;
}

- (MTLTextureDescriptor*)textureDescriptor {
  if (_textureDescriptor == nil) {
    _textureDescriptor = [[MTLTextureDescriptor alloc] init];
    _textureDescriptor.textureType = MTLTextureType2D;
    _textureDescriptor.usage = MTLTextureUsageShaderRead | MTLTextureUsageShaderWrite | MTLTextureUsageRenderTarget;
    _textureDescriptor.width = (NSUInteger)_drawableSize.width;
    _textureDescriptor.height = (NSUInteger)_drawableSize.height;
  }
  return _textureDescriptor;
}

- (void)resetTextures {
  _textureDescriptor = nil;
  _colorTexture = nil;
  _depthTexture = nil;
}

- (CGSize)drawableSize {
  return _drawableSize;
}

- (void)setDrawableSize:(CGSize)drawableSize {
  auto drawableWidth = (NSUInteger)drawableSize.width;
  auto drawableHeight = (NSUInteger)drawableSize.height;
  if (_textureDescriptor.width != drawableWidth || _textureDescriptor.height != drawableHeight) {
    _drawableSize = drawableSize;
    if (_textureDescriptor != nil) {
      [self resetTextures];
    }
  }
}

- (id<MTLTexture>)colorTexture {
  if (_colorTexture == nil) {
    self.textureDescriptor.pixelFormat = MTLPixelFormatBGRA8Unorm;
    _colorTexture = [self.device newTextureWithDescriptor:self.textureDescriptor];
  }
  return _colorTexture;
}

- (id<MTLTexture>)depthTexture {
  if (_depthTexture == nil) {
    self.textureDescriptor.pixelFormat = MTLPixelFormatDepth32Float;
    _depthTexture = [self.device newTextureWithDescriptor:self.textureDescriptor];
  }
  return _depthTexture;
}

- (instancetype)init {
  self = [super init];
  _engine = std::make_unique<seen::Engine>();
  return self;
}

- (void)runPackage:(SeenPackage*)package {
  // clang-format off
  [self runPackage:package withCompletionHandler:^(BOOL){}];
  // clang-format on
}

- (void)runPackage:(SeenPackage*)package withCompletionHandler:(void (^)(BOOL))handler {
  NSUInteger byteLength = package.scriptData.length;
  std::vector<std::byte> cppBytes;
  cppBytes.reserve(byteLength);
  [package.scriptData getBytes:cppBytes.data() length:byteLength];
  _engine->RunModule(cppBytes).Then(handler);
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec {
  _engine->Draw(timeDeltaMillisec);
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(void (^)(void))handler {
  _engine->Draw(timeDeltaMillisec).Then(handler);
}

- (void)reset {
  _engine->Reset();
}

- (void)resetWithCompletionHandler:(void (^)(void))handler {
  _engine->Reset().Then(handler);
}

@end

#endif

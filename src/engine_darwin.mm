/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#import <SeenKit/SeenEngine.h>

#if SEEN_BUILD_DARWIN

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

- (seen::Engine*)engine {
  if (!_engine) {
    _engine = std::make_unique<seen::Engine>();
  }
  return _engine.get();
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
  if (_textureDescriptor == nil || _textureDescriptor.width != drawableWidth ||
      _textureDescriptor.height != drawableHeight) {
    _drawableSize = drawableSize;
    [self resetTextures];
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

- (void)runPackage:(SeenPackage*)package {
  // clang-format off
  [self runPackage:package withCompletionHandler:^(BOOL){}];
  // clang-format on
}

- (void)runPackage:(SeenPackage*)package withCompletionHandler:(void (^)(BOOL))handler {
  NSUInteger byteLength = package.module.length;
  std::vector<std::byte> cpp_bytes;
  cpp_bytes.reserve(byteLength);
  [package.module getBytes:cpp_bytes.data() length:byteLength];
  self.engine->RunModule(cpp_bytes).Then(handler);
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec {
  self.engine->Draw(timeDeltaMillisec);
}

- (void)draw:(NSTimeInterval)timeDeltaMillisec withCompletionHandler:(void (^)(void))handler {
  self.engine->Draw(timeDeltaMillisec).Then(handler);
}

- (void)reset {
  self.engine->Reset();
}

- (void)resetWithCompletionHandler:(void (^)(void))handler {
  self.engine->Reset().Then(handler);
}

@end

#endif

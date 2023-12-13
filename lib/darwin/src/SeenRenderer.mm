// Created by Autokaka (qq1909698494@gmail.com) on 2023/12/11.

#import "SeenRenderer.h"

@implementation SeenRenderer

- (CAMetalLayer*)lockLayer {
  __strong CAMetalLayer* _Nullable strongLayer = self.layer;
  return strongLayer;
}

@end

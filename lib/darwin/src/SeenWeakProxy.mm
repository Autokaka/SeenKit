// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/03.

#import "SeenWeakProxy.h"

@interface SeenWeakProxy ()

@property(atomic, weak, nullable) id weakPtr;

@end

@implementation SeenWeakProxy

- (instancetype)initWithId:(id)ptr {
  self = [super init];
  if (self != nil) {
    self.weakPtr = ptr;
  }
  return self;
}

- (id)lock {
  __strong id strongPtr = self.weakPtr;
  return strongPtr;
}

@end

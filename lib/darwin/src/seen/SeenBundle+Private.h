// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/07.

#import <SeenKit/SeenBundle.h>

#include "seen/bundle.h"

NS_ASSUME_NONNULL_BEGIN

@interface SeenBundle (Private)

@property(readonly, assign, nonatomic) seen::Bundle::Ptr cppBundle;

@end

NS_ASSUME_NONNULL_END

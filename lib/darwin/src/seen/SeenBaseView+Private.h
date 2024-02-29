// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/28.

#import "ViewMacros.h"

#import <QuartzCore/CAMetalLayer.h>
#import <SeenKit/SeenEngine.h>
#import <SeenKit/SeenView.h>

NS_ASSUME_NONNULL_BEGIN

@interface SeenBaseView ()

SEEN_VIEW_INIT_COMMON_DECL;

@property(nonatomic, strong, nullable) SeenEngine* engine;

@end

NS_ASSUME_NONNULL_END

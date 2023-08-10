/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#import <Foundation/Foundation.h>

#include "SeenBuildConfig.h"

SEEN_EXPORT @interface SeenEngine : NSObject

- (instancetype)initWithBundleData:(NSData*)bundleData;

@end

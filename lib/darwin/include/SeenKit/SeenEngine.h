// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>

#import "SeenBundle.h"
#import "SeenMacros.h"

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenEngine : NSObject

@property(nonatomic, weak, nullable) NSObject* view;
@property(nonatomic, assign, getter=isRunning) BOOL running;
@property(nonatomic, strong, readonly) SeenBundle* bundle;

- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithBundle:(nullable SeenBundle*)bundle NS_DESIGNATED_INITIALIZER;

- (void)updateDrawable;

@end

NS_ASSUME_NONNULL_END

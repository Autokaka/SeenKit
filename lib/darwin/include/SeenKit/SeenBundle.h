// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <Foundation/Foundation.h>

#import "SeenMacros.h"

NS_ASSUME_NONNULL_BEGIN

SEEN_EXPORT @interface SeenBundle : NSObject

@property(nonatomic, readonly, copy) NSString* entryPath;
@property(nonatomic, readonly, copy) NSString* assetsDirectory;

- (instancetype)init NS_UNAVAILABLE;
- (nullable instancetype)initWithPath:(nullable NSString*)path NS_DESIGNATED_INITIALIZER;

@end

NS_ASSUME_NONNULL_END

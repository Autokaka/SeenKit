// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <SeenKit/SeenBundle.h>

#include "seen/bundle.h"

NS_ASSUME_NONNULL_BEGIN

@interface SeenBundle (Private)

@property(readonly, assign, nonatomic) seen::Bundle::Ptr cppBundle;

@end

NS_ASSUME_NONNULL_END

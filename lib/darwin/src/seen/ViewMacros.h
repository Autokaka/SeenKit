// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#define SEEN_VIEW_INIT_COMMON_DECL -(void)initCommon

#define SEEN_VIEW_INIT_COMMON_IMPL                   \
  -(instancetype)initWithFrame : (CGRect)frameRect { \
    self = [super initWithFrame:frameRect];          \
    if (self != nil) {                               \
      [self initCommon];                             \
    }                                                \
    return self;                                     \
  }                                                  \
  -(instancetype)initWithCoder : (NSCoder*)coder {   \
    self = [super initWithCoder:coder];              \
    if (self != nil) {                               \
      [self initCommon];                             \
    }                                                \
    return self;                                     \
  }                                                  \
  -(void)initCommon

// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#import <TargetConditionals.h>

#ifdef __cplusplus
#define SEEN_EXTERN extern "C"
#else
#define SEEN_EXTERN extern
#endif

#define SEEN_EXPORT SEEN_EXTERN __attribute__((visibility("default")))

#if TARGET_OS_IOS || TARGET_OS_TV || TARGET_OS_WATCH
#define SEEN_BUILD_IOS 1
#else
#define SEEN_BUILD_MACOS 1
#endif

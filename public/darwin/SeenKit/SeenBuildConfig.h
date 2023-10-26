// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/28.

#pragma once

#include <TargetConditionals.h>

#ifdef __cplusplus
#define SEEN_EXTERN extern "C"
#else
#define SEEN_EXTERN extern
#endif

#define SEEN_EXPORT SEEN_EXTERN __attribute__((visibility("default")))

#if TARGET_OS_IOS || TARGET_OS_TV
#define SEEN_BUILD_IOS 1
#define SEEN_OS_VIEW UIView
#define SEEN_OS_DISPLAY_LINK CADisplayLink
#else
#define SEEN_BUILD_MACOS 1
#define SEEN_OS_VIEW NSView
#define SEEN_OS_DISPLAY_LINK CVDisplayLink
#endif

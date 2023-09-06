// Created by Autokaka (qq1909698494@gmail.com) on 2023/02/28.

#pragma once

#include <TargetConditionals.h>

#ifdef __cplusplus
#define SEEN_EXPORT extern "C" __attribute__((visibility("default")))
#else
#define SEEN_EXPORT extern __attribute__((visibility("default")))
#endif

#if TARGET_OS_IOS || TARGET_OS_TV
#define SEEN_BUILD_IOS 1
#define SEEN_OS_VIEW UIView
#define SEEN_OS_DISPLAY_LINK CADisplayLink
#else
#define SEEN_BUILD_MACOS 1
#define SEEN_OS_VIEW NSView
#define SEEN_OS_DISPLAY_LINK CVDisplayLink
#endif

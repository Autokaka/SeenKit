// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if SEEN_BUILD_MACOS

#import <CoreVideo/CoreVideo.h>

#include "seen/base/deferred_task.h"
#include "seen/base/time_delta.h"
#include "seen/base/time_point.h"
#include "seen/pal/pal.h"

namespace seen::pal {

namespace {

double CVTimeStampToSeconds(const CVTimeStamp& timestamp) {
  return static_cast<double>(timestamp.videoTime) / timestamp.videoTimeScale;
}

CVReturn DisplayLinkCallback(CVDisplayLinkRef display_link,
                             const CVTimeStamp* now,
                             const CVTimeStamp* output_time,
                             CVOptionFlags flags_in,
                             CVOptionFlags* flags_out,
                             void* display_link_context) {
  CVDisplayLinkStop(display_link);
  auto* on_vsync = reinterpret_cast<VsyncCallback*>(display_link_context);
  DeferredTask defer([on_vsync]() { delete on_vsync; });
  auto target_sec = CVTimeStampToSeconds(*output_time);
  auto now_sec = CVTimeStampToSeconds(*now);
  auto time_delta = TimeDelta::FromSecondsF(target_sec - now_sec);
  (*on_vsync)(TimePoint::Now() + time_delta);
  return kCVReturnSuccess;
}

}  // namespace

void* vsync_waiter_create() {
  CVDisplayLinkRef display_link;
  CVDisplayLinkCreateWithActiveCGDisplays(&display_link);
  return display_link;
}

void vsync_waiter_release(void* handle) {
  auto* display_link = reinterpret_cast<CVDisplayLinkRef>(handle);
  CVDisplayLinkStop(display_link);
  CVDisplayLinkRelease(display_link);
}

void vsync_waiter_await(void* handle, VsyncCallback callback) {
  auto* display_link = reinterpret_cast<CVDisplayLinkRef>(handle);
  auto* callback_ptr = new VsyncCallback(std::move(callback));
  CVDisplayLinkSetOutputCallback(display_link, DisplayLinkCallback, callback_ptr);
  CVDisplayLinkStart(display_link);
}

}  // namespace seen::pal

#endif

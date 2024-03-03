// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>
#import <SeenKit/SeenView.h>
#import <dispatch/dispatch.h>
#import <sys/syslog.h>
#include <algorithm>
#include <cmath>

#include "seen/base/logger.h"
#include "seen/pal/pal.h"

namespace seen::pal {

#pragma mark - seen/base/logger.h

void log(int level, const char* message) {
  switch (static_cast<LogLevel>(level)) {
    case LogLevel::kDebug:
      return syslog(LOG_DEBUG, "%s", message);
    case LogLevel::kInfo:
      return syslog(LOG_INFO, "%s", message);
    case LogLevel::kWarn:
      return syslog(LOG_WARNING, "%s", message);
    case LogLevel::kError:
      return syslog(LOG_ERR, "%s", message);
    case LogLevel::kFatal:
      return syslog(LOG_CRIT, "%s", message);
      break;
  }
}

#pragma mark - seen/base/worker_driver.h

bool worker_driver_is_platform_driver() {
  return NSThread.isMainThread == YES;
}

void platform_worker_driver_dispatch_async(const TimePoint& time_point, Closure task) {
  auto delay = time_point - TimePoint::Now();
  __block auto task_block = std::move(task);
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delay.ToNanoseconds()), dispatch_get_main_queue(), ^{
    task_block();
  });
}

#pragma mark - seen/engine.h
void* engine_alloc_drawable(const void* view) {
  auto* typed_view = reinterpret_cast<SeenView*>(view);
  auto* layer = [CAMetalLayer layer];
  typed_view.wantsLayer = YES;
  typed_view.layer = layer;
  typed_view.layer.contentsScale = engine_get_device_pixel_ratio(view);
  return layer;
}

void engine_free_drawable(const void* view, void* drawable) {
  auto* typed_view = reinterpret_cast<SeenView*>(view);
  auto* layer = reinterpret_cast<CAMetalLayer*>(drawable);
  [typed_view.layer removeFromSuperlayer];
  typed_view.layer = nil;
  [layer release];
}

void engine_get_drawable_client_size(const void* view, float* out_width, float* out_height) {
  auto* typed_view = (SeenView*)(view);
  auto* layer = (CAMetalLayer*)(typed_view.layer);
  auto size = typed_view.bounds.size;
  auto width = std::max(0.0, std::ceil(size.width));
  auto height = std::max(0.0, std::ceil(size.height));
  *out_width = static_cast<float>(width);
  *out_height = static_cast<float>(height);
}

float engine_get_device_pixel_ratio(const void* view) {
  auto* typed_view = reinterpret_cast<SeenView*>(view);
  auto scale = typed_view.window.screen.backingScaleFactor;
  return static_cast<float>(scale);
}

#pragma mark - seen/mod/gpu.h
WGPUSurface gpu_surface_create(WGPUInstance instance, const void* drawable) {
  auto* layer = reinterpret_cast<CAMetalLayer*>(drawable);
  WGPUSurfaceDescriptorFromMetalLayer metalLayerDescriptor;
  metalLayerDescriptor.chain.next = nullptr;
  metalLayerDescriptor.chain.sType = WGPUSType_SurfaceDescriptorFromMetalLayer;
  metalLayerDescriptor.layer = layer;

  WGPUSurfaceDescriptor surfaceDescriptor;
  surfaceDescriptor.nextInChain = &metalLayerDescriptor.chain;
  surfaceDescriptor.label = nullptr;
  return wgpuInstanceCreateSurface(instance, &surfaceDescriptor);
}

const char* gpu_get_preferred_texture_format() {
  return "bgra8unorm";
}

WGPUBackendType gpu_get_preferred_backend() {
  return WGPUBackendType_Metal;
}

#pragma mark - seen/mod/drawable.h
void drawable_resize(const void* drawable, float width, float height) {
  auto* layer = reinterpret_cast<CAMetalLayer*>(drawable);
  layer.drawableSize = CGSizeMake(width, height);
}

}  // namespace seen::pal

#endif  // SEEN_BUILD_DARWIN

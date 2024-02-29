// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>
#import <dispatch/dispatch.h>
#import <sys/syslog.h>
#include <algorithm>
#include <cmath>

#import "seen/SeenBaseView+Private.h"
#include "seen/base/logger.h"
#include "seen/pal/pal.h"

namespace seen::pal {

#pragma mark - seen/base/logger.h

void log(int level, const char* message) {
  switch (static_cast<CFLogLevel>(level)) {
    case CFLogLevel::kDebug:
      return syslog(LOG_DEBUG, "%s", message);
    case CFLogLevel::kInfo:
      return syslog(LOG_INFO, "%s", message);
    case CFLogLevel::kWarn:
      return syslog(LOG_WARNING, "%s", message);
    case CFLogLevel::kError:
      return syslog(LOG_ERR, "%s", message);
    case CFLogLevel::kFatal:
      return syslog(LOG_CRIT, "%s", message);
      break;
  }
}

#pragma mark - seen/base/worker_driver.h

bool worker_driver_is_platform_driver() {
  return NSThread.isMainThread == YES;
}

void platform_worker_driver_dispatch_async(const TimePoint& time_point, CFClosure task) {
  auto delay = time_point - TimePoint::Now();
  __block auto task_block = std::move(task);
  dispatch_after(dispatch_time(DISPATCH_TIME_NOW, delay.ToNanoseconds()), dispatch_get_main_queue(), ^{
    task_block();
  });
}

#pragma mark - seen/mod/seen.h
WGPUSurface seen_surface_create(WGPUInstance instance, const void* drawable) {
  auto* view = reinterpret_cast<SeenBaseView*>(drawable);
  WGPUSurfaceDescriptorFromMetalLayer metalLayerDescriptor;
  metalLayerDescriptor.chain.next = nullptr;
  metalLayerDescriptor.chain.sType = WGPUSType_SurfaceDescriptorFromMetalLayer;
  metalLayerDescriptor.layer = view.layer;

  WGPUSurfaceDescriptor surfaceDescriptor;
  surfaceDescriptor.nextInChain = &metalLayerDescriptor.chain;
  surfaceDescriptor.label = nullptr;
  return wgpuInstanceCreateSurface(instance, &surfaceDescriptor);
}

#pragma mark - seen/engine.h
void engine_alloc_drawable(const void* drawable) {
  auto* view = reinterpret_cast<SeenBaseView*>(drawable);
  view.wantsLayer = YES;
  view.layer = [CAMetalLayer layer];
  view.layer.contentsScale = engine_get_device_pixel_ratio(drawable);
}

void engine_free_drawable(const void* drawable) {
  auto* view = reinterpret_cast<SeenBaseView*>(drawable);
  [view.layer removeFromSuperlayer];
  view.layer = nil;
}

void engine_update_drawable(const void* drawable, std::int64_t* updated_width, std::int64_t* updated_height) {
  auto* view = (SeenBaseView*)(drawable);
  auto* layer = (CAMetalLayer*)(view.layer);
  auto size = view.bounds.size;
  auto scale = engine_get_device_pixel_ratio(drawable);
  auto width = std::max(0.0, std::ceil(size.width * scale));
  auto height = std::max(0.0, std::ceil(size.height * scale));
  layer.drawableSize = CGSizeMake(width, height);
  *updated_width = static_cast<std::int64_t>(width);
  *updated_height = static_cast<std::int64_t>(height);
}

double engine_get_device_pixel_ratio(const void* drawable) {
  auto* view = reinterpret_cast<SeenBaseView*>(drawable);
  return view.window.screen.backingScaleFactor;
}

}  // namespace seen::pal

#endif  // SEEN_BUILD_DARWIN

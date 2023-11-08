// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#import <Foundation/Foundation.h>
#import <QuartzCore/CAMetalLayer.h>

#import "SeenWeakProxy.h"

#include <dispatch/dispatch.h>
#include <sys/syslog.h>

#include "seen/base/logger.h"
#include "seen/pal/pal.h"

namespace seen::pal {

#pragma mark - seen/base/logger.h

void log(int level, const char* message) {
  switch (static_cast<CFLogLevel>(level)) {
    case CFLogLevel::kInfo:
      return syslog(LOG_INFO, "%s", message);
    case CFLogLevel::kWarn:
      return syslog(LOG_WARNING, "%s", message);
    case CFLogLevel::kError:
      return syslog(LOG_ERR, "%s", message);
    case CFLogLevel::kFatal:
      return syslog(LOG_CRIT, "%s", message);
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

#pragma mark - renderer

void renderer_release(void* renderer_handle) {
  CFRelease(renderer_handle);
}

const void* renderer_drawable_lock(const std::shared_ptr<void>& renderer) {
  return [static_cast<SeenWeakProxy*>(renderer.get()) lock];
}

glm::vec2 renderer_get_drawable_size(const void* drawable_handle) {
  auto* layer = static_cast<CAMetalLayer*>(drawable_handle);
  if (layer == nullptr) {
    return {0, 0};
  }
  auto drawable_size = layer.drawableSize;
  return {drawable_size.width, drawable_size.height};
}

}  // namespace seen::pal

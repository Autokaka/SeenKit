// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#import <Foundation/Foundation.h>
#include <dispatch/dispatch.h>
#include <sys/syslog.h>

#import "SeenRenderer.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/pal/pal.h"
#include "seen/scene/component/sprite.h"
#include "seen/scene/scene.h"

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
glm::vec2 renderer_get_drawable_size(void* renderer_handle) {
  auto* renderer = reinterpret_cast<SeenRenderer*>(renderer_handle);
  auto drawable_size = renderer.drawableSize;
  return {drawable_size.width, drawable_size.height};
}

void renderer_draw_scene(void* renderer_handle, const Scene* scene) {
  auto* renderer = reinterpret_cast<SeenRenderer*>(renderer_handle);
  auto background_color = scene->background_color.Get();
  auto red = background_color.r;
  auto green = background_color.g;
  auto blue = background_color.b;
  auto alpha = background_color.a;
  [renderer drawScene:MTLClearColorMake(red, green, blue, alpha)];
}

void renderer_draw_node(void* renderer_handle, const scene::Node::Ptr& node) {
  auto component = node->component.Get();
  if (!component) {
    return;
  }
  auto* renderer = reinterpret_cast<SeenRenderer*>(renderer_handle);
  if (component->class_name == scene::kSpriteClassName) {
    auto sprite = std::static_pointer_cast<scene::Sprite>(component);
  }
}

void renderer_present_drawable(void* renderer_handle) {
  auto* renderer = reinterpret_cast<SeenRenderer*>(renderer_handle);
  [renderer presentDrawable];
}

void renderer_release(void* renderer_handle) {
  CFRelease(renderer_handle);
}

}  // namespace seen::pal

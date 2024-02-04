// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#if SEEN_BUILD_DARWIN

#import <Foundation/Foundation.h>
#include <dispatch/dispatch.h>
#include <simd/simd.h>
#include <sys/syslog.h>
#include <glm/gtx/matrix_transform_2d.hpp>

#import "SeenRenderer.h"
#include "glm/ext/matrix_clip_space.hpp"
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
    auto bounds = node->bounds.Get();
    // get screen transform from 'drawable_size'
    auto screen_transform = glm::ortho(0.0F, renderer.drawableSize.width, 0.0F, renderer.drawableSize.height);
    auto transform = node->scene_transform.Get();
    auto bottom_left = transform * glm::vec3{bounds.GetMinX(), bounds.GetMinY(), 1};
    auto left_top = transform * glm::vec3{bounds.GetMinX(), bounds.GetMaxY(), 1};
    auto top_right = transform * glm::vec3{bounds.GetMaxX(), bounds.GetMaxY(), 1};
    auto bottom_right = transform * glm::vec3{bounds.GetMaxX(), bounds.GetMinY(), 1};
    auto color = sprite->color.Get();
    std::vector<DefaultVertexInput> vertices{
        {.position = simd_make_float2(left_top.x, left_top.y)},
        {.position = simd_make_float2(bottom_left.x, bottom_left.y)},
        {.position = simd_make_float2(bottom_right.x, bottom_right.y)},
        {.position = simd_make_float2(bottom_right.x, bottom_right.y)},
        {.position = simd_make_float2(top_right.x, top_right.y)},
        {.position = simd_make_float2(left_top.x, left_top.y)},
    };
    DefaultUniformInput uniform{.color = simd_make_float3(color.r, color.g, color.b)};
    [renderer drawSpriteNode:vertices uniform:uniform];
    return;
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

#endif  // SEEN_BUILD_DARWIN

// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.
// Platform Abstraction Layer (PAL)

#pragma once

#include <glm/glm.hpp>

#include "seen/base/time_point.h"
#include "seen/base/types.h"
#include "seen/scene/node.h"
#include "seen/scene/scene.h"

namespace seen::pal {

#pragma mark - seen/base/logger.h
void log(int level, const char* message);

#pragma mark - seen/base/worker_driver.h
bool worker_driver_is_platform_driver();
void platform_worker_driver_dispatch_async(const TimePoint& time_point, CFClosure task);

#pragma mark - renderer
glm::vec2 renderer_get_drawable_size(void* renderer_handle);
void renderer_draw_scene(void* renderer_handle, const Scene* scene);
void renderer_draw_node(void* renderer_handle, const scene::Node::Ptr& node);
void renderer_release(void* renderer_handle);

}  // namespace seen::pal

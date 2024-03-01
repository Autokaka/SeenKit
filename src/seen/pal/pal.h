// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.
// Platform Abstraction Layer (PAL)

#pragma once

#include <wgpu/wgpu.h>
#include <optional>

#include "seen/base/time_point.h"
#include "seen/base/types.h"

namespace seen::pal {

namespace fs = std::filesystem;

#pragma mark - seen/base/logger.h
void log(int level, const char* message);

#pragma mark - seen/base/worker_driver.h
bool worker_driver_is_platform_driver();
void platform_worker_driver_dispatch_async(const TimePoint& time_point, CFClosure task);

#pragma mark - seen/base/vsync_waiter.h
void* vsync_waiter_create();
void vsync_waiter_release(void* handle);
using VsyncCallback = std::function<void(std::optional<TimePoint> frame_display_time)>;
void vsync_waiter_await(void* handle, VsyncCallback callback);

#pragma mark - seen/engine.h
void* engine_alloc_drawable(const void* view);
void engine_free_drawable(const void* view, void* drawable);
void engine_get_drawable_client_size(const void* view, float* out_width, float* out_height);
float engine_get_device_pixel_ratio(const void* view);

#pragma mark - seen/mod/gpu.h
WGPUSurface gpu_surface_create(WGPUInstance instance, const void* drawable);
const char* gpu_get_preferred_texture_format(WGPUInstance instance);

#pragma mark - seen/mod/drawable.h
void drawable_resize(const void* drawable, float width, float height);

}  // namespace seen::pal

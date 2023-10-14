// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.
// Platform Abstraction Layer (PAL)

#pragma once

#include "seen/foundation/time_point.h"
#include "seen/foundation/types.h"

namespace seen::pal {

#pragma mark - seen/foundation/logger.h
void log(int level, const char* message);

#pragma mark - seen/foundation/worker_driver.h
bool worker_driver_is_platform_driver();
void platform_worker_driver_dispatch_async(const TimePoint& time_point, CFClosure task);

}  // namespace seen::pal

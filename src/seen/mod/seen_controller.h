// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/24.

#pragma once

#include "seen/base/rx_value.h"
#include "seen/mod/drawable_metrics.h"

namespace seen::mod {

struct SeenController {
  rx::Value<bool> is_running;
  rx::Value<const void*> drawable;
  rx::Value<DrawableMetrics> drawable_metrics;
};

}  // namespace seen::mod

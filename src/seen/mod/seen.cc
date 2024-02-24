// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/seen.h"
#include "seen/base/logger.h"

namespace seen::mod {

Seen::Seen() : version(SEEN_VERSION), gpu_(nullptr) {
  SEEN_DEBUG("Create Seen instance.");
  is_running.OnNext([this](bool is_running) {
    SEEN_DEBUG("Is running: {}", is_running);
    if (is_running) {
      on_running_state_changed_callback(is_running);
    }
  });
  drawable.OnNext([this](const void* drawable) {
    SEEN_DEBUG("Drawable available: {}", drawable != nullptr);
    if (on_drawable_changed_callback) {
      on_drawable_changed_callback(drawable != nullptr);
    }
  });
  drawable_metrics.OnNext([this](const DrawableMetrics& metrics) {
    SEEN_DEBUG("Drawable metrics: {}", metrics.ToString());
    if (on_drawable_metrics_changed_callback) {
      on_drawable_metrics_changed_callback(metrics);
    }
  });
}

Seen::~Seen() {
  SEEN_DEBUG("Release Seen instance.");
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Seen::Log(const sol::variadic_args& args) const {
  std::string message;
  for (auto&& arg : args) {
    std::string desc = luaL_tolstring(arg.lua_state(), arg.stack_index(), nullptr);
    message += desc + " ";
  }
  SEEN_INFO("{}", message);
}

GPU::Ptr Seen::GetGPU() {
  gpu_ || (gpu_ = GPU::Create());
  return gpu_;
}

}  // namespace seen::mod

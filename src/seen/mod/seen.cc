// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/seen.h"
#include "seen/base/logger.h"

namespace seen::mod {

Seen::Seen(const CFWorker::Ptr& runner) : Object(Object::Name::kSeen), version(SEEN_VERSION), runner_(runner) {
  SEEN_DEBUG("Create Seen instance.");
  Reset();
  is_running_.OnNext([this](bool is_running) {
    SEEN_DEBUG("Is running: {}", is_running);
    if (on_running_state_changed_callback) {
      on_running_state_changed_callback(is_running);
    }
  });
  drawable_.OnNext([this](const void* drawable) {
    SEEN_DEBUG("Drawable available: {}", drawable != nullptr);
    GetGPU()->drawable_ = drawable;
    if (on_drawable_changed_callback) {
      on_drawable_changed_callback(drawable != nullptr);
    }
  });
  drawable_metrics_.OnNext([this](const DrawableMetrics& metrics) {
    SEEN_DEBUG("Drawable metrics update");
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

FramePacer::Ptr Seen::GetFramePacer() {
  frame_pacer_ || (frame_pacer_ = FramePacer::Create(runner_.lock()));
  return frame_pacer_;
}

bool Seen::isRunning() const {
  return is_running_.Get();
}

bool Seen::IsDrawableAvailable() const {
  return drawable_.Get() != nullptr;
}

DrawableMetrics Seen::GetDrawableMetrics() const {
  return drawable_metrics_.Get();
}

void Seen::Reset() {
  is_running_ = false;
  gpu_ = nullptr;
  drawable_metrics_ = DrawableMetrics();
  drawable_ = nullptr;
}

}  // namespace seen::mod

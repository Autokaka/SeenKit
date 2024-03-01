// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/seen.h"
#include "seen/base/logger.h"

namespace seen::mod {

Seen::Seen(const CFWorker::Ptr& runner)
    : Object(Object::Name::kSeen),
      version(SEEN_VERSION),
      runner_(runner),
      is_running_(false),
      gpu_(nullptr),
      drawable_ref_(nullptr),
      client_size_({0, 0}),
      device_pixel_ratio_(1) {
  SEEN_DEBUG("Create Seen instance.");
  is_running_.OnNext([this](bool is_running) {
    SEEN_DEBUG("Is running: {}", is_running);
    if (on_running_state_changed_callback) {
      on_running_state_changed_callback(is_running);
    }
  });
  drawable_ref_.OnNext([this](const void* drawable_ref) {
    SEEN_DEBUG("Drawable available: {}", drawable != nullptr);
    GetGPU()->drawable_ref_ = drawable_ref;
    if (on_drawable_changed_callback) {
      on_drawable_changed_callback(drawable_ref != nullptr);
    }
  });
  client_size_.OnNext([this](const Vec<2>& size) {
    SEEN_DEBUG("Client size: {}, {}", size[0], size[1]);
    if (on_client_size_changed_callback) {
      on_client_size_changed_callback(size);
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

Drawable::Ptr Seen::GetDrawable() {
  return GetGPU()->drawable_;
}

Vec<2> Seen::GetClientSize() const {
  return client_size_.Get();
}

double Seen::GetDevicePixelRatio() const {
  return device_pixel_ratio_.Get();
}

}  // namespace seen::mod

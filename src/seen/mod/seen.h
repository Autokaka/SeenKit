// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <functional>
#include <sol/sol.hpp>

#include "seen/mod/gpu.h"
#include "seen/mod/macros.h"
#include "seen/mod/seen_controller.h"

SEEN_CLASS_FORWARD_DECL(Engine);

namespace seen::mod {

class Seen final : private SeenController {
 public:
  friend class seen::Engine;
  using Ptr = std::shared_ptr<Seen>;
  using RunningStateCallback = std::function<void(bool running)>;
  using DrawabeCallback = std::function<void(bool has_drawable)>;
  using DrawableMetricsCallback = std::function<void(const DrawableMetrics& metrics)>;

  explicit Seen();
  ~Seen();

  void Log(const sol::variadic_args& args) const;
  GPU::Ptr GetGPU();

  RunningStateCallback on_running_state_changed_callback;
  DrawabeCallback on_drawable_changed_callback;
  DrawableMetricsCallback on_drawable_metrics_changed_callback;
  const char* version;

 private:
  GPU::Ptr gpu_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Seen);
};

}  // namespace seen::mod

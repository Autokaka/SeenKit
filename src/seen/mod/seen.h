// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <array>
#include <functional>
#include <sol/sol.hpp>

#include "seen/base/rx_value.h"
#include "seen/base/worker.h"
#include "seen/mod/frame_pacer.h"
#include "seen/mod/gpu.h"
#include "seen/mod/macros.h"
#include "seen/mod/object.h"

SEEN_CLASS_FORWARD_DECL(Engine);

namespace seen::mod {

class Seen final : public Object {
 public:
  friend class seen::Engine;
  using Ptr = std::shared_ptr<Seen>;
  using Vec2 = std::array<std::int64_t, 2>;
  using RunningStateCallback = std::function<void(bool running)>;
  using DrawabeCallback = std::function<void(bool is_drawable_available)>;
  using DrawableSizeCallback = std::function<void(const Vec2& metrics)>;

  explicit Seen(const CFWorker::Ptr& runner);
  ~Seen() override;

  void Log(const sol::variadic_args& args) const;
  GPU::Ptr GetGPU();
  FramePacer::Ptr GetFramePacer();
  [[nodiscard]] bool isRunning() const;
  [[nodiscard]] bool IsDrawableAvailable() const;
  [[nodiscard]] Vec2 GetDrawableSize() const;
  [[nodiscard]] double GetDevicePixelRatio() const;

  RunningStateCallback on_running_state_changed_callback;
  DrawabeCallback on_drawable_changed_callback;
  DrawableSizeCallback on_drawable_size_changed_callback;

  const char* version;

 private:
  void Reset();

  rx::Value<bool> is_running_;
  rx::Value<const void*> drawable_;
  rx::Value<Vec2> drawable_size_;
  const CFWorker::WeakPtr runner_;
  FramePacer::Ptr frame_pacer_;
  GPU::Ptr gpu_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Seen);
};

}  // namespace seen::mod

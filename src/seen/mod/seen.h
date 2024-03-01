// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <functional>
#include <sol/sol.hpp>

#include "seen/base/rx_value.h"
#include "seen/base/worker.h"
#include "seen/mod/drawable.h"
#include "seen/mod/frame_pacer.h"
#include "seen/mod/gpu.h"
#include "seen/mod/macros.h"
#include "seen/mod/object.h"
#include "seen/mod/types.h"

SEEN_CLASS_FORWARD_DECL(Engine);

namespace seen::mod {

class Seen final : public Object {
 public:
  friend class seen::Engine;
  using Ptr = std::shared_ptr<Seen>;
  using RunningStateCallback = std::function<void(bool running)>;
  using ClientSizeCallback = std::function<void(const Vec<2>& size)>;
  using DrawableCallback = std::function<void(bool is_available)>;

  explicit Seen(const Worker::Ptr& runner);
  ~Seen() override;

  void Log(const sol::variadic_args& args) const;
  GPU::Ptr GetGPU();
  FramePacer::Ptr GetFramePacer();
  [[nodiscard]] bool isRunning() const;
  [[nodiscard]] Drawable::Ptr GetDrawable();
  [[nodiscard]] Vec<2> GetClientSize() const;
  [[nodiscard]] double GetDevicePixelRatio() const;

  RunningStateCallback on_running_state_changed_callback;
  DrawableCallback on_drawable_changed_callback;
  ClientSizeCallback on_client_size_changed_callback;

  const char* version;

 private:
  rx::Value<bool> is_running_;
  rx::Value<const void*> drawable_ref_;
  rx::Value<Vec<2>> client_size_;
  rx::Value<float> device_pixel_ratio_;
  const Worker::WeakPtr runner_;
  FramePacer::Ptr frame_pacer_;
  GPU::Ptr gpu_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Seen);
};

}  // namespace seen::mod

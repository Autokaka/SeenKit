// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <functional>
#include <memory>

#include "seen/base/class_ext.h"
#include "seen/base/rx_value.h"
#include "seen/mod/drawable.h"
#include "seen/mod/gpu_adapter.h"
#include "seen/mod/gpu_texture_format.h"
#include "seen/mod/object.h"

namespace seen::mod {

class GPU final : public Object {
 public:
  friend class Seen;
  using Ptr = std::shared_ptr<GPU>;
  using RequestAdapterCallback = std::function<void(GPUAdapter::Ptr)>;

  struct RequestAdapterOptions {
    GPUAdapter::PowerPref::Type power_preference = nullptr;
  };

  static GPU::Ptr Create();
  explicit GPU(WGPUInstance wgpu);
  ~GPU() override;

  void RequestAdapter(const RequestAdapterOptions& options, RequestAdapterCallback callback);
  void RequestAdapter(const RequestAdapterCallback& callback);

  GPUTextureFormat::Type GetPreferredTextureFormat() const;

 private:
  void DoRequestAdapter(const RequestAdapterOptions& options, RequestAdapterCallback callback);

  WGPUInstance wgpu_;
  rx::Value<const void*> drawable_ref_;
  Drawable::Ptr drawable_;
  Drawable::Surface surface_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPU);
};

}  // namespace seen::mod

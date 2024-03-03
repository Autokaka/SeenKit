// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <functional>
#include <memory>
#include <set>

#include "seen/base/class_ext.h"
#include "seen/base/rx_value.h"
#include "seen/mod/drawable.h"
#include "seen/mod/gpu_adapter.h"
#include "seen/mod/gpu_power_preference.h"
#include "seen/mod/gpu_texture_format.h"
#include "seen/mod/object.h"

namespace seen::mod {

struct GPURequestAdapterOptions {
  GPUPowerPreference::Type power_preference = nullptr;
  bool force_fallback_adapter = false;

  static WGPURequestAdapterOptions ToWGPU(const GPURequestAdapterOptions& options, WGPUSurface surface);
};

using GPURequestAdapterCallback = std::function<void(GPUAdapter::Ptr)>;

using WGSLLanguageFeatures = std::set<const char*>;

class GPU final : public Object {
 public:
  friend class Seen;
  using Ptr = std::shared_ptr<GPU>;

  static GPU::Ptr Create();
  explicit GPU(WGPUInstance wgpu);
  ~GPU() override;

  void RequestAdapter(const GPURequestAdapterOptions& options, GPURequestAdapterCallback callback);
  void RequestAdapter(const GPURequestAdapterCallback& callback);

  GPUTextureFormat::Type GetPreferredTextureFormat() const;
  WGSLLanguageFeatures GetWGSLLanguageFeatures() const;

 private:
  void DoRequestAdapter(const GPURequestAdapterOptions& options, GPURequestAdapterCallback callback);

  WGPUInstance wgpu_;
  rx::Value<const void*> drawable_ref_;
  Drawable::Ptr drawable_;
  Drawable::Surface surface_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPU);
};

}  // namespace seen::mod

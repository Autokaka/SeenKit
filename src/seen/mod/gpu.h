// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <wgpu/wgpu.h>
#include <functional>
#include <memory>

#include "seen/base/class_ext.h"
#include "seen/mod/gpu_adapter.h"
#include "seen/mod/gpu_texture_format.h"

namespace seen::mod {

class GPU final {
 public:
  using Ptr = std::shared_ptr<GPU>;
  using RequestAdapterCallback = std::function<void(GPUAdapter::Ptr)>;

  struct RequestAdapterOptions {
    static const RequestAdapterOptions kDefault;

    GPUAdapter::PowerPrefT power_preference;
  };

  static GPU::Ptr Create();
  explicit GPU(WGPUInstance wgpu);
  ~GPU();

  void RequestAdapter(const RequestAdapterOptions& options, const RequestAdapterCallback& callback);
  void RequestAdapter(const RequestAdapterCallback& callback);

  [[nodiscard]] GPUTextureFormatT GetPreferredDrawableFormat() const;

 private:
  void DoRequestAdapter(const RequestAdapterOptions& options = RequestAdapterOptions::kDefault,
                        const RequestAdapterCallback& callback = nullptr);

  WGPUInstance wgpu_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPU);
};

}  // namespace seen::mod

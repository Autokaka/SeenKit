// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <wgpu/wgpu.h>
#include <functional>
#include <memory>

#include "seen/base/class_ext.h"
#include "seen/mod/gpu_adapter.h"
#include "seen/mod/object.h"

namespace seen::mod {

class GPU final : public Object {
 public:
  friend class Seen;
  using Ptr = std::shared_ptr<GPU>;
  using RequestAdapterCallback = std::function<void(GPUAdapter::Ptr)>;

  struct RequestAdapterOptions {
    GPUAdapter::TPowerPref power_preference = nullptr;
  };

  static GPU::Ptr Create(const void* drawable = nullptr);
  explicit GPU(WGPUInstance wgpu, const void* drawable = nullptr);
  ~GPU() override;

  void RequestAdapter(const RequestAdapterOptions& options, RequestAdapterCallback callback);
  void RequestAdapter(const RequestAdapterCallback& callback);

 private:
  void DoRequestAdapter(const RequestAdapterOptions& options, RequestAdapterCallback callback);

  WGPUInstance wgpu_;
  const void* drawable_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(GPU);
};

}  // namespace seen::mod

// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <string>

namespace seen::mod {

struct GPUFeatureName {
  using Type = const char*;

  static constexpr Type kDepthClipControl = "depth-clip-control";
  static constexpr Type kDepth32floatStencil8 = "depth32float-stencil8";
  static constexpr Type kTextureCompressionBc = "texture-compression-bc";
  static constexpr Type kTextureCompressionEtc2 = "texture-compression-etc2";
  static constexpr Type kTextureCompressionAstc = "texture-compression-astc";
  static constexpr Type kTimestampQuery = "timestamp-query";
  static constexpr Type kIndirectFirstInstance = "indirect-first-instance";
  static constexpr Type kShaderF16 = "shader-f16";
  static constexpr Type kRg11b10ufloatRenderable = "rg11b10ufloat-renderable";
  static constexpr Type kBgra8unormStorage = "bgra8unorm-storage";
  static constexpr Type kFloat32Filterable = "float32-filterable";

  static GPUFeatureName::Type FromWGPU(const WGPUFeatureName& name) {
    switch (name) {
      case WGPUFeatureName_DepthClipControl:
        return GPUFeatureName::kDepthClipControl;
      case WGPUFeatureName_Depth32FloatStencil8:
        return GPUFeatureName::kDepth32floatStencil8;
      case WGPUFeatureName_TextureCompressionBC:
        return GPUFeatureName::kTextureCompressionBc;
      case WGPUFeatureName_TextureCompressionETC2:
        return GPUFeatureName::kTextureCompressionEtc2;
      case WGPUFeatureName_TextureCompressionASTC:
        return GPUFeatureName::kTextureCompressionAstc;
      case WGPUFeatureName_TimestampQuery:
        return GPUFeatureName::kTimestampQuery;
      case WGPUFeatureName_IndirectFirstInstance:
        return GPUFeatureName::kIndirectFirstInstance;
      case WGPUFeatureName_ShaderF16:
        return GPUFeatureName::kShaderF16;
      case WGPUFeatureName_RG11B10UfloatRenderable:
        return GPUFeatureName::kRg11b10ufloatRenderable;
      case WGPUFeatureName_BGRA8UnormStorage:
        return GPUFeatureName::kBgra8unormStorage;
      case WGPUFeatureName_Float32Filterable:
        return GPUFeatureName::kFloat32Filterable;
      case WGPUFeatureName_Undefined:
      case WGPUFeatureName_Force32:
        return nullptr;
    }
  }

  static WGPUFeatureName ToWGPU(const std::string& name) {
    if (name == GPUFeatureName::kDepthClipControl) {
      return WGPUFeatureName_DepthClipControl;
    }
    if (name == GPUFeatureName::kDepth32floatStencil8) {
      return WGPUFeatureName_Depth32FloatStencil8;
    }
    if (name == GPUFeatureName::kTextureCompressionBc) {
      return WGPUFeatureName_TextureCompressionBC;
    }
    if (name == GPUFeatureName::kTextureCompressionEtc2) {
      return WGPUFeatureName_TextureCompressionETC2;
    }
    if (name == GPUFeatureName::kTextureCompressionAstc) {
      return WGPUFeatureName_TextureCompressionASTC;
    }
    if (name == GPUFeatureName::kTimestampQuery) {
      return WGPUFeatureName_TimestampQuery;
    }
    if (name == GPUFeatureName::kIndirectFirstInstance) {
      return WGPUFeatureName_IndirectFirstInstance;
    }
    if (name == GPUFeatureName::kShaderF16) {
      return WGPUFeatureName_ShaderF16;
    }
    if (name == GPUFeatureName::kRg11b10ufloatRenderable) {
      return WGPUFeatureName_RG11B10UfloatRenderable;
    }
    if (name == GPUFeatureName::kBgra8unormStorage) {
      return WGPUFeatureName_BGRA8UnormStorage;
    }
    if (name == GPUFeatureName::kFloat32Filterable) {
      return WGPUFeatureName_Float32Filterable;
    }
    return WGPUFeatureName_Undefined;
  }

  static WGPUFeatureName ToWGPU(const char* name) {  //
    return ToWGPU(std::string(name != nullptr ? name : ""));
  }
};

static bool IsGPUFeatureName(const std::string& some) {
  return some == GPUFeatureName::kDepthClipControl ||         //
         some == GPUFeatureName::kDepth32floatStencil8 ||     //
         some == GPUFeatureName::kTextureCompressionBc ||     //
         some == GPUFeatureName::kTextureCompressionEtc2 ||   //
         some == GPUFeatureName::kTextureCompressionAstc ||   //
         some == GPUFeatureName::kTimestampQuery ||           //
         some == GPUFeatureName::kIndirectFirstInstance ||    //
         some == GPUFeatureName::kShaderF16 ||                //
         some == GPUFeatureName::kRg11b10ufloatRenderable ||  //
         some == GPUFeatureName::kBgra8unormStorage ||        //
         some == GPUFeatureName::kFloat32Filterable;
}

static bool IsGPUFeatureName(const char* some) {
  return some != nullptr && IsGPUFeatureName(std::string(some));
}

}  // namespace seen::mod

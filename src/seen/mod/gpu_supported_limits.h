// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <unordered_map>

namespace seen::mod {

struct GPUSupportedLimits : WGPULimits {
  static WGPULimits ToWGPU(std::unordered_map<std::string, std::uint32_t> map) {
    WGPULimits limits;
    std::memset(&limits, 0, sizeof(WGPULimits));
    limits.maxTextureDimension1D = map["maxTextureDimension1D"];
    limits.maxTextureDimension2D = map["maxTextureDimension2D"];
    limits.maxTextureDimension3D = map["maxTextureDimension3D"];
    limits.maxTextureArrayLayers = map["maxTextureArrayLayers"];
    limits.maxBindGroups = map["maxBindGroups"];
    limits.maxBindGroupsPlusVertexBuffers = map["maxBindGroupsPlusVertexBuffers"];
    limits.maxBindingsPerBindGroup = map["maxBindingsPerBindGroup"];
    limits.maxDynamicUniformBuffersPerPipelineLayout = map["maxDynamicUniformBuffersPerPipelineLayout"];
    limits.maxDynamicStorageBuffersPerPipelineLayout = map["maxDynamicStorageBuffersPerPipelineLayout"];
    limits.maxSampledTexturesPerShaderStage = map["maxSampledTexturesPerShaderStage"];
    limits.maxSamplersPerShaderStage = map["maxSamplersPerShaderStage"];
    limits.maxStorageBuffersPerShaderStage = map["maxStorageBuffersPerShaderStage"];
    limits.maxStorageTexturesPerShaderStage = map["maxStorageTexturesPerShaderStage"];
    limits.maxUniformBuffersPerShaderStage = map["maxUniformBuffersPerShaderStage"];
    limits.maxUniformBufferBindingSize = map["maxUniformBufferBindingSize"];
    limits.maxStorageBufferBindingSize = map["maxStorageBufferBindingSize"];
    limits.minUniformBufferOffsetAlignment = map["minUniformBufferOffsetAlignment"];
    limits.minStorageBufferOffsetAlignment = map["minStorageBufferOffsetAlignment"];
    limits.maxVertexBuffers = map["maxVertexBuffers"];
    limits.maxBufferSize = map["maxBufferSize"];
    limits.maxVertexAttributes = map["maxVertexAttributes"];
    limits.maxVertexBufferArrayStride = map["maxVertexBufferArrayStride"];
    limits.maxInterStageShaderComponents = map["maxInterStageShaderComponents"];
    limits.maxInterStageShaderVariables = map["maxInterStageShaderVariables"];
    limits.maxColorAttachments = map["maxColorAttachments"];
    return limits;
  }
};

}  // namespace seen::mod

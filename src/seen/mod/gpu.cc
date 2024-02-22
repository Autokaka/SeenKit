// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/gpu.h"
#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/mod/gpu_adapter.h"

namespace seen::mod {

const GPU::RequestAdapterOptions GPU::RequestAdapterOptions::kDefault = {
    .power_preference = nullptr,
};

GPU::Ptr GPU::Create() {
  WGPUInstanceDescriptor desc;
  desc.nextInChain = nullptr;
  auto* wgpu = wgpuCreateInstance(&desc);
  return wgpu != nullptr ? std::make_shared<GPU>(wgpu) : nullptr;
}

GPU::GPU(WGPUInstance wgpu) : wgpu_(wgpu) {
  SEEN_INFO("Create GPU instance.");
  SEEN_ASSERT(wgpu);
}

GPU::~GPU() {
  SEEN_INFO("Release GPU instance.");
  wgpuInstanceRelease(wgpu_);
}

void GPU::RequestAdapter(const RequestAdapterOptions& options, const RequestAdapterCallback& callback) {
  DoRequestAdapter(options, callback);
}

void GPU::RequestAdapter(const RequestAdapterCallback& callback) {
  DoRequestAdapter({.power_preference = nullptr}, callback);
}

void GPU::DoRequestAdapter(const RequestAdapterOptions& options, const RequestAdapterCallback& callback) {
  SEEN_ASSERT(callback);

  WGPURequestAdapterOptions wgpu_options;
  wgpu_options.nextInChain = nullptr;
  // TODO(Autokaka): Get surface from system.
  wgpu_options.compatibleSurface = nullptr;
  wgpu_options.powerPreference = WGPUPowerPreference_Undefined;
  wgpu_options.forceFallbackAdapter = 0;
  wgpu_options.backendType = WGPUBackendType_Undefined;
  if (options.power_preference == GPUAdapter::PowerPref::kLowPower) {
    wgpu_options.powerPreference = WGPUPowerPreference_LowPower;
  } else if (options.power_preference == GPUAdapter::PowerPref::kHighPerformance) {
    wgpu_options.powerPreference = WGPUPowerPreference_HighPerformance;
  }
  auto* callback_ptr = new RequestAdapterCallback(callback);
  auto c_callback = [](WGPURequestAdapterStatus status, WGPUAdapter adapter, char const*, void* user_data) {
    auto* callback_ptr = reinterpret_cast<RequestAdapterCallback*>(user_data);
    CFDeferredTask defer([callback_ptr]() { delete callback_ptr; });
    auto success = status == WGPURequestAdapterStatus_Success && adapter != nullptr;
    (*callback_ptr)(success ? std::make_shared<GPUAdapter>(adapter) : nullptr);
  };
  wgpuInstanceRequestAdapter(wgpu_, &wgpu_options, c_callback, callback_ptr);
}

GPUTextureFormatT GPU::GetPreferredDrawableFormat() const {
  // TODO(Autokaka): Get surface from system.
  auto format = wgpuSurfaceGetPreferredFormat(nullptr, nullptr);
  switch (format) {
    case WGPUTextureFormat_Undefined:
    case WGPUTextureFormat_Force32:
      return nullptr;
    case WGPUTextureFormat_R8Unorm:
      return GPUTextureFormat::kR8Unorm;
    case WGPUTextureFormat_R8Snorm:
      return GPUTextureFormat::kR8Snorm;
    case WGPUTextureFormat_R8Uint:
      return GPUTextureFormat::kR8Uint;
    case WGPUTextureFormat_R8Sint:
      return GPUTextureFormat::kR8Sint;
    case WGPUTextureFormat_R16Uint:
      return GPUTextureFormat::kR16Uint;
    case WGPUTextureFormat_R16Sint:
      return GPUTextureFormat::kR16Sint;
    case WGPUTextureFormat_R16Float:
      return GPUTextureFormat::kR16Float;
    case WGPUTextureFormat_RG8Unorm:
      return GPUTextureFormat::kRG8Unorm;
    case WGPUTextureFormat_RG8Snorm:
      return GPUTextureFormat::kRG8Snorm;
    case WGPUTextureFormat_RG8Uint:
      return GPUTextureFormat::kRG8Uint;
    case WGPUTextureFormat_RG8Sint:
      return GPUTextureFormat::kRG8Sint;
    case WGPUTextureFormat_R32Float:
      return GPUTextureFormat::kR32Float;
    case WGPUTextureFormat_R32Uint:
      return GPUTextureFormat::kR32Uint;
    case WGPUTextureFormat_R32Sint:
      return GPUTextureFormat::kR32Sint;
    case WGPUTextureFormat_RG16Uint:
      return GPUTextureFormat::kRG16Uint;
    case WGPUTextureFormat_RG16Sint:
      return GPUTextureFormat::kRG16Sint;
    case WGPUTextureFormat_RG16Float:
      return GPUTextureFormat::kRG16Float;
    case WGPUTextureFormat_RGBA8Unorm:
      return GPUTextureFormat::kRGBA8Unorm;
    case WGPUTextureFormat_RGBA8UnormSrgb:
      return GPUTextureFormat::kRGBA8UnormSrgb;
    case WGPUTextureFormat_RGBA8Snorm:
      return GPUTextureFormat::kRGBA8Snorm;
    case WGPUTextureFormat_RGBA8Uint:
      return GPUTextureFormat::kRGBA8Uint;
    case WGPUTextureFormat_RGBA8Sint:
      return GPUTextureFormat::kRGBA8Sint;
    case WGPUTextureFormat_BGRA8Unorm:
      return GPUTextureFormat::kBGRA8Unorm;
    case WGPUTextureFormat_BGRA8UnormSrgb:
      return GPUTextureFormat::kBGRA8UnormSrgb;
    case WGPUTextureFormat_RGB10A2Uint:
      return GPUTextureFormat::kRGB10A2Uint;
    case WGPUTextureFormat_RGB10A2Unorm:
      return GPUTextureFormat::kRGB10A2Unorm;
    case WGPUTextureFormat_RG11B10Ufloat:
      return GPUTextureFormat::kRG11B10Ufloat;
    case WGPUTextureFormat_RGB9E5Ufloat:
      return GPUTextureFormat::kRGB9E5Ufloat;
    case WGPUTextureFormat_RG32Float:
      return GPUTextureFormat::kRG32Float;
    case WGPUTextureFormat_RG32Uint:
      return GPUTextureFormat::kRG32Uint;
    case WGPUTextureFormat_RG32Sint:
      return GPUTextureFormat::kRG32Sint;
    case WGPUTextureFormat_RGBA16Uint:
      return GPUTextureFormat::kRGBA16Uint;
    case WGPUTextureFormat_RGBA16Sint:
      return GPUTextureFormat::kRGBA16Sint;
    case WGPUTextureFormat_RGBA16Float:
      return GPUTextureFormat::kRGBA16Float;
    case WGPUTextureFormat_RGBA32Float:
      return GPUTextureFormat::kRGBA32Float;
    case WGPUTextureFormat_RGBA32Uint:
      return GPUTextureFormat::kRGBA32Uint;
    case WGPUTextureFormat_RGBA32Sint:
      return GPUTextureFormat::kRGBA32Sint;
    case WGPUTextureFormat_Stencil8:
      return GPUTextureFormat::kStencil8;
    case WGPUTextureFormat_Depth16Unorm:
      return GPUTextureFormat::kDepth16Unorm;
    case WGPUTextureFormat_Depth24Plus:
      return GPUTextureFormat::kDepth24Plus;
    case WGPUTextureFormat_Depth24PlusStencil8:
      return GPUTextureFormat::kDepth24PlusStencil8;
    case WGPUTextureFormat_Depth32Float:
      return GPUTextureFormat::kDepth32Float;
    case WGPUTextureFormat_Depth32FloatStencil8:
      return GPUTextureFormat::kDepth32FloatStencil8;
    case WGPUTextureFormat_BC1RGBAUnorm:
      return GPUTextureFormat::kBC1RGBAUnorm;
    case WGPUTextureFormat_BC1RGBAUnormSrgb:
      return GPUTextureFormat::kBC1RGBAUnormSrgb;
    case WGPUTextureFormat_BC2RGBAUnorm:
      return GPUTextureFormat::kBC2RGBAUnorm;
    case WGPUTextureFormat_BC2RGBAUnormSrgb:
      return GPUTextureFormat::kBC2RGBAUnormSrgb;
    case WGPUTextureFormat_BC3RGBAUnorm:
      return GPUTextureFormat::kBC3RGBAUnorm;
    case WGPUTextureFormat_BC3RGBAUnormSrgb:
      return GPUTextureFormat::kBC3RGBAUnormSrgb;
    case WGPUTextureFormat_BC4RUnorm:
      return GPUTextureFormat::kBC4RUnorm;
    case WGPUTextureFormat_BC4RSnorm:
      return GPUTextureFormat::kBC4RSnorm;
    case WGPUTextureFormat_BC5RGUnorm:
      return GPUTextureFormat::kBC5RGUnorm;
    case WGPUTextureFormat_BC5RGSnorm:
      return GPUTextureFormat::kBC5RGSnorm;
    case WGPUTextureFormat_BC6HRGBUfloat:
      return GPUTextureFormat::kBC6HRGBUfloat;
    case WGPUTextureFormat_BC6HRGBFloat:
      return GPUTextureFormat::kBC6HRGBFloat;
    case WGPUTextureFormat_BC7RGBAUnorm:
      return GPUTextureFormat::kBC7RGBAUnorm;
    case WGPUTextureFormat_BC7RGBAUnormSrgb:
      return GPUTextureFormat::kBC7RGBAUnormSrgb;
    case WGPUTextureFormat_ETC2RGB8Unorm:
      return GPUTextureFormat::kETC2RGB8Unorm;
    case WGPUTextureFormat_ETC2RGB8UnormSrgb:
      return GPUTextureFormat::kETC2RGB8UnormSrgb;
    case WGPUTextureFormat_ETC2RGB8A1Unorm:
      return GPUTextureFormat::kETC2RGB8A1Unorm;
    case WGPUTextureFormat_ETC2RGB8A1UnormSrgb:
      return GPUTextureFormat::kETC2RGB8A1UnormSrgb;
    case WGPUTextureFormat_ETC2RGBA8Unorm:
      return GPUTextureFormat::kETC2RGBA8Unorm;
    case WGPUTextureFormat_ETC2RGBA8UnormSrgb:
      return GPUTextureFormat::kETC2RGBA8UnormSrgb;
    case WGPUTextureFormat_EACR11Unorm:
      return GPUTextureFormat::kEACR11Unorm;
    case WGPUTextureFormat_EACR11Snorm:
      return GPUTextureFormat::kEACR11Snorm;
    case WGPUTextureFormat_EACRG11Unorm:
      return GPUTextureFormat::kEACRG11Unorm;
    case WGPUTextureFormat_EACRG11Snorm:
      return GPUTextureFormat::kEACRG11Snorm;
    case WGPUTextureFormat_ASTC4x4Unorm:
      return GPUTextureFormat::kASTC4x4Unorm;
    case WGPUTextureFormat_ASTC4x4UnormSrgb:
      return GPUTextureFormat::kASTC4x4UnormSrgb;
    case WGPUTextureFormat_ASTC5x4Unorm:
      return GPUTextureFormat::kASTC5x4Unorm;
    case WGPUTextureFormat_ASTC5x4UnormSrgb:
      return GPUTextureFormat::kASTC5x4UnormSrgb;
    case WGPUTextureFormat_ASTC5x5Unorm:
      return GPUTextureFormat::kASTC5x5Unorm;
    case WGPUTextureFormat_ASTC5x5UnormSrgb:
      return GPUTextureFormat::kASTC5x5UnormSrgb;
    case WGPUTextureFormat_ASTC6x5Unorm:
      return GPUTextureFormat::kASTC6x5Unorm;
    case WGPUTextureFormat_ASTC6x5UnormSrgb:
      return GPUTextureFormat::kASTC6x5UnormSrgb;
    case WGPUTextureFormat_ASTC6x6Unorm:
      return GPUTextureFormat::kASTC6x6Unorm;
    case WGPUTextureFormat_ASTC6x6UnormSrgb:
      return GPUTextureFormat::kASTC6x6UnormSrgb;
    case WGPUTextureFormat_ASTC8x5Unorm:
      return GPUTextureFormat::kASTC8x5Unorm;
    case WGPUTextureFormat_ASTC8x5UnormSrgb:
      return GPUTextureFormat::kASTC8x5UnormSrgb;
    case WGPUTextureFormat_ASTC8x6Unorm:
      return GPUTextureFormat::kASTC8x6Unorm;
    case WGPUTextureFormat_ASTC8x6UnormSrgb:
      return GPUTextureFormat::kASTC8x6UnormSrgb;
    case WGPUTextureFormat_ASTC8x8Unorm:
      return GPUTextureFormat::kASTC8x8Unorm;
    case WGPUTextureFormat_ASTC8x8UnormSrgb:
      return GPUTextureFormat::kASTC8x8UnormSrgb;
    case WGPUTextureFormat_ASTC10x5Unorm:
      return GPUTextureFormat::kASTC10x5Unorm;
    case WGPUTextureFormat_ASTC10x5UnormSrgb:
      return GPUTextureFormat::kASTC10x5UnormSrgb;
    case WGPUTextureFormat_ASTC10x6Unorm:
      return GPUTextureFormat::kASTC10x6Unorm;
    case WGPUTextureFormat_ASTC10x6UnormSrgb:
      return GPUTextureFormat::kASTC10x6UnormSrgb;
    case WGPUTextureFormat_ASTC10x8Unorm:
      return GPUTextureFormat::kASTC10x8Unorm;
    case WGPUTextureFormat_ASTC10x8UnormSrgb:
      return GPUTextureFormat::kASTC10x8UnormSrgb;
    case WGPUTextureFormat_ASTC10x10Unorm:
      return GPUTextureFormat::kASTC10x10Unorm;
    case WGPUTextureFormat_ASTC10x10UnormSrgb:
      return GPUTextureFormat::kASTC10x10UnormSrgb;
    case WGPUTextureFormat_ASTC12x10Unorm:
      return GPUTextureFormat::kASTC12x10Unorm;
    case WGPUTextureFormat_ASTC12x10UnormSrgb:
      return GPUTextureFormat::kASTC12x10UnormSrgb;
    case WGPUTextureFormat_ASTC12x12Unorm:
      return GPUTextureFormat::kASTC12x12Unorm;
    case WGPUTextureFormat_ASTC12x12UnormSrgb:
      return GPUTextureFormat::kASTC12x12UnormSrgb;
    default:
      SEEN_ASSERT(false);
  }
  return nullptr;
}

}  // namespace seen::mod

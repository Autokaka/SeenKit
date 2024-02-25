// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/23.

#pragma once

#include <wgpu/wgpu.h>

namespace seen::mod {

using TGPUTextureFormat = const char*;
struct GPUTextureFormat final {
  static TGPUTextureFormat From(const WGPUTextureFormat& format);

  static constexpr TGPUTextureFormat kR8Unorm = "r8unorm";
  static constexpr TGPUTextureFormat kR8Snorm = "r8snorm";
  static constexpr TGPUTextureFormat kR8Uint = "r8uint";
  static constexpr TGPUTextureFormat kR8Sint = "r8sint";
  static constexpr TGPUTextureFormat kR16Uint = "r16uint";
  static constexpr TGPUTextureFormat kR16Sint = "r16sint";
  static constexpr TGPUTextureFormat kR16Float = "r16float";
  static constexpr TGPUTextureFormat kRG8Unorm = "rg8unorm";
  static constexpr TGPUTextureFormat kRG8Snorm = "rg8snorm";
  static constexpr TGPUTextureFormat kRG8Uint = "rg8uint";
  static constexpr TGPUTextureFormat kRG8Sint = "rg8sint";
  static constexpr TGPUTextureFormat kR32Float = "r32float";
  static constexpr TGPUTextureFormat kR32Uint = "r32uint";
  static constexpr TGPUTextureFormat kR32Sint = "r32sint";
  static constexpr TGPUTextureFormat kRG16Uint = "rg16uint";
  static constexpr TGPUTextureFormat kRG16Sint = "rg16sint";
  static constexpr TGPUTextureFormat kRG16Float = "rg16float";
  static constexpr TGPUTextureFormat kRGBA8Unorm = "rgba8unorm";
  static constexpr TGPUTextureFormat kRGBA8UnormSrgb = "rgba8unorm-srgb";
  static constexpr TGPUTextureFormat kRGBA8Snorm = "rgba8snorm";
  static constexpr TGPUTextureFormat kRGBA8Uint = "rgba8uint";
  static constexpr TGPUTextureFormat kRGBA8Sint = "rgba8sint";
  static constexpr TGPUTextureFormat kBGRA8Unorm = "bgra8unorm";
  static constexpr TGPUTextureFormat kBGRA8UnormSrgb = "bgra8unorm-srgb";
  static constexpr TGPUTextureFormat kRGB10A2Uint = "rgb10a2uint";
  static constexpr TGPUTextureFormat kRGB10A2Unorm = "rgb10a2unorm";
  static constexpr TGPUTextureFormat kRG11B10Ufloat = "rg11b10ufloat";
  static constexpr TGPUTextureFormat kRGB9E5Ufloat = "rgb9e5ufloat";
  static constexpr TGPUTextureFormat kRG32Float = "rg32float";
  static constexpr TGPUTextureFormat kRG32Uint = "rg32uint";
  static constexpr TGPUTextureFormat kRG32Sint = "rg32sint";
  static constexpr TGPUTextureFormat kRGBA16Uint = "rgba16uint";
  static constexpr TGPUTextureFormat kRGBA16Sint = "rgba16sint";
  static constexpr TGPUTextureFormat kRGBA16Float = "rgba16float";
  static constexpr TGPUTextureFormat kRGBA32Float = "rgba32float";
  static constexpr TGPUTextureFormat kRGBA32Uint = "rgba32uint";
  static constexpr TGPUTextureFormat kRGBA32Sint = "rgba32sint";
  static constexpr TGPUTextureFormat kStencil8 = "stencil8";
  static constexpr TGPUTextureFormat kDepth16Unorm = "depth16unorm";
  static constexpr TGPUTextureFormat kDepth24Plus = "depth24plus";
  static constexpr TGPUTextureFormat kDepth24PlusStencil8 = "depth24plus-stencil8";
  static constexpr TGPUTextureFormat kDepth32Float = "depth32float";
  static constexpr TGPUTextureFormat kDepth32FloatStencil8 = "depth32float-stencil8";
  static constexpr TGPUTextureFormat kBC1RGBAUnorm = "bc1-rgba-unorm";
  static constexpr TGPUTextureFormat kBC1RGBAUnormSrgb = "bc1-rgba-unorm-srgb";
  static constexpr TGPUTextureFormat kBC2RGBAUnorm = "bc2-rgba-unorm";
  static constexpr TGPUTextureFormat kBC2RGBAUnormSrgb = "bc2-rgba-unorm-srgb";
  static constexpr TGPUTextureFormat kBC3RGBAUnorm = "bc3-rgba-unorm";
  static constexpr TGPUTextureFormat kBC3RGBAUnormSrgb = "bc3-rgba-unorm-srgb";
  static constexpr TGPUTextureFormat kBC4RUnorm = "bc4-r-unorm";
  static constexpr TGPUTextureFormat kBC4RSnorm = "bc4-r-snorm";
  static constexpr TGPUTextureFormat kBC5RGUnorm = "bc5-rg-unorm";
  static constexpr TGPUTextureFormat kBC5RGSnorm = "bc5-rg-snorm";
  static constexpr TGPUTextureFormat kBC6HRGBUfloat = "bc6h-rgb-ufloat";
  static constexpr TGPUTextureFormat kBC6HRGBFloat = "bc6h-rgb-float";
  static constexpr TGPUTextureFormat kBC7RGBAUnorm = "bc7-rgba-unorm";
  static constexpr TGPUTextureFormat kBC7RGBAUnormSrgb = "bc7-rgba-unorm-srgb";
  static constexpr TGPUTextureFormat kETC2RGB8Unorm = "etc2-rgb8unorm";
  static constexpr TGPUTextureFormat kETC2RGB8UnormSrgb = "etc2-rgb8unorm-srgb";
  static constexpr TGPUTextureFormat kETC2RGB8A1Unorm = "etc2-rgb8a1unorm";
  static constexpr TGPUTextureFormat kETC2RGB8A1UnormSrgb = "etc2-rgb8a1unorm-srgb";
  static constexpr TGPUTextureFormat kETC2RGBA8Unorm = "etc2-rgba8unorm";
  static constexpr TGPUTextureFormat kETC2RGBA8UnormSrgb = "etc2-rgba8unorm-srgb";
  static constexpr TGPUTextureFormat kEACR11Unorm = "eac-r11unorm";
  static constexpr TGPUTextureFormat kEACR11Snorm = "eac-r11snorm";
  static constexpr TGPUTextureFormat kEACRG11Unorm = "eac-rg11unorm";
  static constexpr TGPUTextureFormat kEACRG11Snorm = "eac-rg11snorm";
  static constexpr TGPUTextureFormat kASTC4x4Unorm = "astc-4x4-unorm";
  static constexpr TGPUTextureFormat kASTC4x4UnormSrgb = "astc-4x4-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC5x4Unorm = "astc-5x4-unorm";
  static constexpr TGPUTextureFormat kASTC5x4UnormSrgb = "astc-5x4-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC5x5Unorm = "astc-5x5-unorm";
  static constexpr TGPUTextureFormat kASTC5x5UnormSrgb = "astc-5x5-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC6x5Unorm = "astc-6x5-unorm";
  static constexpr TGPUTextureFormat kASTC6x5UnormSrgb = "astc-6x5-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC6x6Unorm = "astc-6x6-unorm";
  static constexpr TGPUTextureFormat kASTC6x6UnormSrgb = "astc-6x6-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC8x5Unorm = "astc-8x5-unorm";
  static constexpr TGPUTextureFormat kASTC8x5UnormSrgb = "astc-8x5-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC8x6Unorm = "astc-8x6-unorm";
  static constexpr TGPUTextureFormat kASTC8x6UnormSrgb = "astc-8x6-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC8x8Unorm = "astc-8x8-unorm";
  static constexpr TGPUTextureFormat kASTC8x8UnormSrgb = "astc-8x8-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC10x5Unorm = "astc-10x5-unorm";
  static constexpr TGPUTextureFormat kASTC10x5UnormSrgb = "astc-10x5-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC10x6Unorm = "astc-10x6-unorm";
  static constexpr TGPUTextureFormat kASTC10x6UnormSrgb = "astc-10x6-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC10x8Unorm = "astc-10x8-unorm";
  static constexpr TGPUTextureFormat kASTC10x8UnormSrgb = "astc-10x8-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC10x10Unorm = "astc-10x10-unorm";
  static constexpr TGPUTextureFormat kASTC10x10UnormSrgb = "astc-10x10-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC12x10Unorm = "astc-12x10-unorm";
  static constexpr TGPUTextureFormat kASTC12x10UnormSrgb = "astc-12x10-unorm-srgb";
  static constexpr TGPUTextureFormat kASTC12x12Unorm = "astc-12x12-unorm";
  static constexpr TGPUTextureFormat kASTC12x12UnormSrgb = "astc-12x12-unorm-srgb";
};

}  // namespace seen::mod

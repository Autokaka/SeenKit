// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/23.

/// <reference types="./gpu_texture_format" />
/// <reference types="./object" />

declare namespace Seen {
  const GPUAdapter: never;

  interface GPUAdapter extends Object<"kGPUAdapter"> {
    readonly preferredTextureFormat: Seen.GPUTextureFormat;
  }
}
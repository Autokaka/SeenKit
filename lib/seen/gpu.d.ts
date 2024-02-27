// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/23.

/// <reference types="./gpu_texture_format" />
/// <reference types="./object" />

declare namespace Seen {
  const GPU: never;

  interface GPU extends Object<"kGPU"> {
    readonly preferredDrawableFormat?: Seen.GPUTextureFormat;

    requestAdapter(options: GPU.RequestAdapterOptions, callback: GPU.RequestAdapterCallback): void;
    requestAdapter(callback: GPU.RequestAdapterCallback): void;
  }

  namespace GPU {
    type RequestAdapterCallback = (this: void, adapter?: GPUAdapter) => void;
    type PowerPreference = "low-power" | "high-performance";
    interface RequestAdapterOptions {
      readonly powerPreference?: PowerPreference;
    }
  }
}

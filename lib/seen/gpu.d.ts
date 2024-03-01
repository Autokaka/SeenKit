// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/23.

/// <reference types="./gpu_adapter" />
/// <reference types="./object" />

declare namespace Seen {
  const GPU: never;

  interface GPU extends Object<"kGPU"> {
    readonly preferredTextureFormat: "rgba8unorm" | "bgra8unorm";

    requestAdapter(options: GPU.RequestAdapterOptions, callback: GPU.RequestAdapterCallback): void;
    requestAdapter(callback: GPU.RequestAdapterCallback): void;
  }

  namespace GPU {
    type RequestAdapterCallback = (this: void, adapter?: GPUAdapter) => void;
    interface RequestAdapterOptions {
      readonly powerPreference?: "low-power" | "high-performance";
    }
  }
}

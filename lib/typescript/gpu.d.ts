// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/23.

/// <reference types="./object" />

declare namespace Seen {
  class GPU extends Object<"kGPU"> {
    private constructor();

    requestAdapter(options: GPU.RequestAdapterOptions, callback: GPU.RequestAdapterCallback): void;
    requestAdapter(callback: GPU.RequestAdapterCallback): void;

    readonly preferredDrawableFormat: Seen.GPUTextureFormat | undefined;
  }

  namespace GPU {
    type RequestAdapterCallback = (this: void, adapter?: GPUAdapter) => void;
    type PowerPreference = "low-power" | "high-performance";
    type RequestAdapterOptions = {
      readonly powerPreference?: PowerPreference;
    };
  }
}

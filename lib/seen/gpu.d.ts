// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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

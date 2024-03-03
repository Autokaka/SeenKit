// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/// <reference types="./gpu_adapter" />
/// <reference types="./object" />

declare namespace Seen {
  const GPU: never;

  type GPURequestAdapterCallback = (this: void, adapter?: GPUAdapter) => void;

  interface GPURequestAdapterOptions {
    readonly powerPreference?: "low-power" | "high-performance";
  }

  interface GPU extends Object<"kGPU"> {
    readonly preferredTextureFormat: "rgba8unorm" | "bgra8unorm";
    readonly wgslLanguageFeatures: Set<string>;

    requestAdapter(options: GPURequestAdapterOptions, callback: GPURequestAdapterCallback): void;
    requestAdapter(callback: GPURequestAdapterCallback): void;
  }
}

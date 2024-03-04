// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

type _Tuple<T, N extends number> = N extends N ? (number extends N ? T[] : _TupleOf<T, N, []>) : never;
type _TupleOf<T, N extends number, R extends unknown[]> = R["length"] extends N ? R : _TupleOf<T, N, [T, ...R]>;

declare namespace Seen {
  type Vec<N extends number> = Readonly<_Tuple<number, N>>;
  type Mat<N extends number> = Readonly<_Tuple<_Tuple<number, N>, N>>;

  const Drawable: never;

  interface Drawable extends Object<"kDrawable"> {
    size: Seen.Vec<2>;
  }

  const FramePacer: never;

  interface FramePacer extends Seen.Object<"kFramePacer"> {
    requestAnimationFrame(callback: FramePacer.FrameCallback): number;
    cancelAnimationFrame(token: number): void;
  }

  namespace FramePacer {
    type FrameCallback = (this: void, nowMillis: number, outputMillis: number) => void;
  }

  const GPUDevice: never;

  interface GPUDevice extends Object<"kGPUDevice"> {
    // TODO(Autokaka):
  }

  interface GPUAdapterInfo {
    readonly vendor: string;
    readonly architecture: string;
    readonly device: string;
    readonly description: string;
  }

  interface GPUQueueDescriptor {
    readonly label?: string;
  }

  interface GPUDeviceDescriptor {
    readonly label?: string;
    readonly requiredFeatures?: GPUFeatureName[];
    readonly requiredLimits?: Partial<GPUSupportedLimits>;
    readonly defaultQueue?: GPUQueueDescriptor;
  }

  interface GPUSupportedLimits {
    readonly maxTextureDimension1D: number;
    readonly maxTextureDimension2D: number;
    readonly maxTextureDimension3D: number;
    readonly maxTextureArrayLayers: number;
    readonly maxBindGroups: number;
    readonly maxBindGroupsPlusVertexBuffers: number;
    readonly maxBindingsPerBindGroup: number;
    readonly maxDynamicUniformBuffersPerPipelineLayout: number;
    readonly maxDynamicStorageBuffersPerPipelineLayout: number;
    readonly maxSampledTexturesPerShaderStage: number;
    readonly maxSamplersPerShaderStage: number;
    readonly maxStorageBuffersPerShaderStage: number;
    readonly maxStorageTexturesPerShaderStage: number;
    readonly maxUniformBuffersPerShaderStage: number;
    readonly maxUniformBufferBindingSize: number;
    readonly maxStorageBufferBindingSize: number;
    readonly minUniformBufferOffsetAlignment: number;
    readonly minStorageBufferOffsetAlignment: number;
    readonly maxVertexBuffers: number;
    readonly maxBufferSize: number;
    readonly maxVertexAttributes: number;
    readonly maxVertexBufferArrayStride: number;
    readonly maxInterStageShaderComponents: number;
    readonly maxInterStageShaderVariables: number;
    readonly maxColorAttachments: number;
  }

  type GPUFeatureName =
    | "depth-clip-control"
    | "depth32float-stencil8"
    | "texture-compression-bc"
    | "texture-compression-etc2"
    | "texture-compression-astc"
    | "timestamp-query"
    | "indirect-first-instance"
    | "shader-f16"
    | "rg11b10ufloat-renderable"
    | "bgra8unorm-storage"
    | "float32-filterable";

  type GPURequestDeviceCallback = (this: void, device: GPUDevice) => void;

  type GPURequestAdapterInfoCallback = (this: void, info: GPUAdapterInfo) => void;

  const GPUAdapter: never;

  interface GPUAdapter extends Object<"kGPUAdapter"> {
    readonly features: GPUFeatureName[];
    readonly limits: GPUSupportedLimits;
    readonly isFallbackAdapter: boolean;

    requestDevice(callback: GPURequestDeviceCallback): void;
    requestDevice(desc: GPUDeviceDescriptor, callback: GPURequestDeviceCallback): void;

    requestAdapterInfo(callback: GPURequestAdapterInfoCallback): void;
  }

  type GPURequestAdapterCallback = (this: void, adapter?: GPUAdapter) => void;

  type GPUPowerPreference = "low-power" | "high-performance";

  interface GPURequestAdapterOptions {
    readonly powerPreference?: GPUPowerPreference;
  }

  type GPUTextureFormat = "rgba8unorm" | "bgra8unorm";

  const GPU: never;

  interface GPU extends Object<"kGPU"> {
    readonly preferredTextureFormat: GPUTextureFormat;
    readonly wgslLanguageFeatures: Set<string>;

    requestAdapter(options: GPURequestAdapterOptions, callback: GPURequestAdapterCallback): void;
    requestAdapter(callback: GPURequestAdapterCallback): void;
  }

  interface Object<N extends keyof Object.Names = "kObject"> {
    readonly className: Object.Names[N];
  }

  namespace Object {
    enum Name {
      kDrawable = "Drawable",
      kFramePacer = "FramePacer",
      kGPUAdapter = "GPUAdapter",
      kGPUDevice = "GPUDevice",
      kGPU = "GPU",
      kSeen = "Seen",
      kObject = "Object",
    }
    type Names = typeof Name;
  }

  type RunningStateChangedCallback = (this: void, isRunning: boolean) => void;
  type DrawableChangedCallback = (this: void, isAvailable: boolean) => void;
  type ClientSizeChangedCallback = (this: void, size: Vec<2>) => void;
}

declare interface Seen extends Seen.Object<"kSeen"> {
  readonly version: string;
  readonly gpu?: Seen.GPU;
  readonly framePacer: Seen.FramePacer;
  readonly isRunning: boolean;
  readonly drawable?: Seen.Drawable;
  readonly clientSize: Seen.Vec<2>;

  onRunningStateChanged?: Seen.RunningStateChangedCallback;
  onDrawableChanged?: Seen.DrawableChangedCallback;
  onClientSizeChanged?: Seen.ClientSizeChangedCallback;

  log(...args: unknown[]): void;
}

declare const seen: Seen;

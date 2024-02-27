// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

/// <reference types="./drawable_metrics" />
/// <reference types="./frame_pacer" />
/// <reference types="./gpu_adapter" />
/// <reference types="./gpu_texture_format" />
/// <reference types="./gpu" />
/// <reference types="./object" />

declare interface Seen extends Seen.Object<"kSeen"> {
  readonly version: string;
  readonly gpu?: Seen.GPU;
  readonly framePacer: Seen.FramePacer;
  readonly isRunning: boolean;
  readonly isDrawableAvailable: boolean;
  readonly drawableMetrics: Seen.DrawableMetrics;

  onRunningStateChanged?: Seen.RunningStateChangedCallback;
  onDrawableChanged?: Seen.DrawableChangedCallback;
  onDrawableMetricsChanged?: Seen.DrawableMetricsChangedCallback;

  log(...args: unknown[]): void;
}

declare namespace Seen {
  type RunningStateChangedCallback = (this: void, isRunning: boolean) => void;
  type DrawableChangedCallback = (this: void, isAvailable: boolean) => void;
  type DrawableMetricsChangedCallback = (this: void, metrics: Seen.DrawableMetrics) => void;
}

declare const seen: Seen;

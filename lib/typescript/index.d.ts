// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

/// <reference types="./drawable_metrics" />
/// <reference types="./frame_pacer" />
/// <reference types="./gpu_adapter" />
/// <reference types="./gpu_texture_format" />
/// <reference types="./gpu" />
/// <reference types="./object" />

declare class Seen implements Seen.Object<"kSeen"> {
  private constructor();

  onRunningStateChanged?: Seen.RunningStateChangedCallback;
  onDrawableChanged?: Seen.DrawableChangedCallback;
  onDrawableMetricsChanged?: Seen.DrawableMetricsChangedCallback;

  log(...args: unknown[]): void;

  createFramePacer(): Seen.FramePacer;

  readonly className: Seen.Object.Name.kSeen;
  readonly version: string;
  readonly gpu: Seen.GPU | undefined;
  readonly isRunning: boolean;
  readonly isDrawableAvailable: boolean;
  readonly drawableMetrics: Seen.DrawableMetrics;
}

declare namespace Seen {
  type RunningStateChangedCallback = (this: void, isRunning: boolean) => void;
  type DrawableChangedCallback = (this: void, isAvailable: boolean) => void;
  type DrawableMetricsChangedCallback = (this: void, metrics: Seen.DrawableMetrics) => void;
}

declare const seen: Seen;

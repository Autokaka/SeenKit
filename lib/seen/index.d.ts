// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

/// <reference types="./frame_pacer" />
/// <reference types="./gpu_adapter" />
/// <reference types="./gpu" />
/// <reference types="./object" />

declare interface Seen extends Seen.Object<"kSeen"> {
  readonly version: string;
  readonly gpu?: Seen.GPU;
  readonly framePacer: Seen.FramePacer;
  readonly isRunning: boolean;
  readonly isDrawableAvailable: boolean;
  readonly drawableSize: Seen.Vec2;

  onRunningStateChanged?: Seen.RunningStateChangedCallback;
  onDrawableChanged?: Seen.DrawableChangedCallback;
  onDrawableSizeChanged?: Seen.DrawableSizeChangedCallback;

  log(...args: unknown[]): void;
}

declare namespace Seen {
  type Vec2 = readonly [number, number];
  type RunningStateChangedCallback = (this: void, isRunning: boolean) => void;
  type DrawableChangedCallback = (this: void, isAvailable: boolean) => void;
  type DrawableSizeChangedCallback = (this: void, size: Vec2) => void;
}

declare const seen: Seen;

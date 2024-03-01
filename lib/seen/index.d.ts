// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/// <reference types="./drawable" />
/// <reference types="./frame_pacer" />
/// <reference types="./gpu_adapter" />
/// <reference types="./gpu" />
/// <reference types="./object" />
/// <reference types="./types" />

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

declare namespace Seen {
  type RunningStateChangedCallback = (this: void, isRunning: boolean) => void;
  type DrawableChangedCallback = (this: void, isAvailable: boolean) => void;
  type ClientSizeChangedCallback = (this: void, size: Vec<2>) => void;
}

declare const seen: Seen;

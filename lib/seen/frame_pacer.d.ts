// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/// <reference types="./object" />

declare namespace Seen {
  const FramePacer: never;

  interface FramePacer extends Seen.Object<"kFramePacer"> {
    requestAnimationFrame(callback: FramePacer.FrameCallback): number;
    cancelAnimationFrame(token: number): void;
  }

  namespace FramePacer {
    type FrameCallback = (this: void, nowMillis: number, outputMillis: number) => void;
  }
}

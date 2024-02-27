// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/25.

/// <reference types="./object" />

declare namespace Seen {
  class FramePacer extends Seen.Object<"kFramePacer"> {
    private constructor();

    requestAnimationFrame(callback: FramePacer.FrameCallback): number;
    cancelAnimationFrame(token: number): void;
  }

  namespace FramePacer {
    interface FrameWindow {
      readonly last: number;
      readonly now: number;
      readonly output: number;
    }

    type FrameCallback = (this: void, time: FrameWindow) => void;
  }
}

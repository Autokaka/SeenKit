// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/25.

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

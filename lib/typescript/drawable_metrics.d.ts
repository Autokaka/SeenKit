// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/24.

/// <reference types="./object" />

declare namespace Seen {
  class DrawableMetrics extends Object<"kDrawableMetrics"> {
    private constructor();

    readonly width: number;
    readonly height: number;
    readonly contentScale: number;
  }
}

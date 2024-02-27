// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/24.

/// <reference types="./object" />

declare namespace Seen {
  const DrawableMetrics: never;

  interface DrawableMetrics extends Object<"kDrawableMetrics"> {
    readonly width: number;
    readonly height: number;
    readonly contentScale: number;
  }
}

// Created by Autokaka (qq1909698494@gmail.com) on 2024/03/01.

/// <reference types="./object" />
/// <reference types="./types" />

declare namespace Seen {
  const Drawable: never;

  interface Drawable extends Object<"kDrawable"> {
    size: Seen.Vec<2>;
  }
}

// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/// <reference types="./object" />
/// <reference types="./types" />

declare namespace Seen {
  const Drawable: never;

  interface Drawable extends Object<"kDrawable"> {
    size: Seen.Vec<2>;
  }
}

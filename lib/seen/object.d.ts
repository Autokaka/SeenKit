// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

declare namespace Seen {
  interface Object<N extends keyof Object.Names = "kObject"> {
    readonly className: Object.Names[N];
  }

  namespace Object {
    enum Name {
      kDrawable = "Drawable",
      kFramePacer = "FramePacer",
      kGPUAdapter = "GPUAdapter",
      kGPU = "GPU",
      kSeen = "Seen",
      kObject = "Object",
    }
    type Names = typeof Name;
  }
}

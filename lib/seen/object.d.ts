// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/26.

declare namespace Seen {
  abstract class Object<N extends keyof Object.Names = "kObject"> {
    constructor(className: Object.Names[N]);

    readonly className: Object.Names[N];
  }

  namespace Object {
    enum Name {
      kDrawableMetrics = "DrawableMetrics",
      kFramePacer = "FramePacer",
      kGPUAdapter = "GPUAdapter",
      kGPU = "GPU",
      kSeen = "Seen",
      kObject = "Object",
    }
    type Names = typeof Name;
  }
}

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/26.

declare namespace Seen {
  interface Object<N extends keyof Object.Names = "kObject"> {
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

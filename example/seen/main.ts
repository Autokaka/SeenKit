// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

/// <reference types="@seen-kit/seen" />

function main(this: unknown) {
  try {
    function test() {}
    seen.log("seen.className:", seen.className);
    seen.log("seen.version:", seen.version);
    seen.log("test:", test);
    seen.log("this:", this);
    seen.log("undefined:", undefined);
    seen.log("null:", null);
    seen.log("0:", 0);
    seen.log("true:", true);
    seen.log("seen.gpu:", seen.gpu);
    seen.log("seen.gpu?.className:", seen.gpu?.className);
    seen.log("typeof seen.gpu:", typeof seen.gpu);
    // seen.log("seen.gpu.prototype:", seen.gpu ? Reflect.getPrototypeOf(seen.gpu) : undefined);
    seen.log("seen.gpu?.constructor:", seen.gpu?.constructor);
    seen.log("Seen.GPU:", Seen.GPU);
    seen.log("typeof Seen.GPU:", typeof Seen.GPU);

    const testGPUAdapter = () => {
      seen.gpu?.requestAdapter((adapter) => {
        seen.log("adapter:", adapter);
        seen.log("adapter?.className:", adapter?.className);
        seen.log("adapter?.preferredDrawableFormat:", adapter?.preferredTextureFormat);
      });
    };

    testGPUAdapter();

    seen.log("seen.isRunning", seen.isRunning);
    seen.log("seen.isDrawableAvailable:", seen.isDrawableAvailable);
    seen.log("seen.drawableMetrics:", seen.drawableMetrics);
    seen.log("seen.onRunningStateChanged", seen.onRunningStateChanged);
    seen.log("seen.onDrawableChanged", seen.onDrawableChanged);
    seen.log("seen.onDrawableMetricsChanged", seen.onDrawableMetricsChanged);
    seen.onRunningStateChanged = (isRunning) => {
      seen.log(`seen.onRunningStateChanged(isRunning=${isRunning})`);
    };
    seen.onDrawableChanged = (isAvailable) => {
      seen.log(`seen.onDrawableChanged(isAvailable=${isAvailable})`);
      if (isAvailable) {
        testGPUAdapter();
      }
    };
    seen.onDrawableMetricsChanged = (metrics) => {
      seen.log(`seen.onDrawableMetricsChanged(metrics=${metrics})`);
    };
    seen.log("seen.onRunningStateChanged", seen.onRunningStateChanged);
    seen.log("seen.onDrawableChanged", seen.onDrawableChanged);
    seen.log("seen.onDrawableMetricsChanged", seen.onDrawableMetricsChanged);

    seen.log("seen.framePacer:", seen.framePacer);
    seen.log("seen.framePacer.className:", seen.framePacer.className);
    seen.log("seen.framePacer.requestAnimationFrame:", seen.framePacer.requestAnimationFrame);
    const onFrame: Seen.FramePacer.FrameCallback = (nowMillis, outputMillis) => {
      seen.log(`onFrame(nowMillis=${nowMillis}, outputMillis=${outputMillis})`);
      // seen.framePacer.requestAnimationFrame((a, b) => onFrame(a, b));
    };
    seen.framePacer.requestAnimationFrame((a, b) => onFrame(a, b));
  } catch (error) {
    seen.log(`Error:`, error);
  }
}

main();

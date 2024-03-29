// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

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
    seen.log("seen.gpu?.preferredTextureFormat:", seen.gpu?.preferredTextureFormat);
    seen.log("seen.gpu?.constructor:", seen.gpu?.constructor);
    seen.log("Seen.GPU:", Seen.GPU);
    seen.log("typeof Seen.GPU:", typeof Seen.GPU);

    const testGPUAdapter = () => {
      seen.gpu?.requestAdapter((adapter) => {
        seen.log("adapter:", adapter);
        seen.log("adapter?.className:", adapter?.className);
      });
    };

    testGPUAdapter();

    seen.log("seen.isRunning", seen.isRunning);
    seen.log("seen.drawable:", seen.drawable);
    seen.log("seen.drawable?.size:", seen.drawable?.size);
    seen.log("seen.onRunningStateChanged", seen.onRunningStateChanged);
    seen.log("seen.onDrawableChanged", seen.onDrawableChanged);
    seen.log("seen.onClientSizeChanged", seen.onClientSizeChanged);
    seen.onRunningStateChanged = (isRunning) => {
      seen.log(`seen.onRunningStateChanged(isRunning=${isRunning})`);
    };
    seen.onDrawableChanged = (isAvailable) => {
      seen.log(`seen.onDrawableChanged(isAvailable=${isAvailable})`);
      if (isAvailable) {
        testGPUAdapter();
      }
    };
    seen.onClientSizeChanged = ([width, height]) => {
      seen.log(`seen.onClientSizeChanged(width=${width}, height=${height})`);
    };
    seen.log("seen.onRunningStateChanged", seen.onRunningStateChanged);
    seen.log("seen.onDrawableChanged", seen.onDrawableChanged);
    seen.log("seen.onClientSizeChanged", seen.onClientSizeChanged);

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

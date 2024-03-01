// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>

namespace seen::mod::GPUTextureFormat {

using Type = const char*;
static constexpr Type kRGBA8Unorm = "rgba8unorm";
static constexpr Type kBGRA8Unorm = "bgra8unorm";

}  // namespace seen::mod::GPUTextureFormat

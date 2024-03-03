// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <sol/sol.hpp>

#include "seen/mod/gpu.h"
#include "seen/mod/gpu_power_preference.h"

namespace seen::runtime::glue {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

void GPURequestAdapter(GPU* gpu, const sol::variadic_args& args) {
  GPURequestAdapterOptions gpu_options;
  if (args[0].get_type() == sol::type::table) {
    auto options = args[0].as<sol::table>();
    auto power_pref = options["powerPreference"];
    if (power_pref.get_type() == sol::type::string) {
      auto pref_str = power_pref.get<std::string>();
      if (IsGPUPowerPreference(pref_str)) {
        gpu_options.power_preference = pref_str.c_str();
      }
    }
    auto force_fallback_adapter = options["forceFallbackAdapter"];
    if (force_fallback_adapter.get_type() == sol::type::boolean) {
      gpu_options.force_fallback_adapter = force_fallback_adapter.get<bool>();
    }
  }
  GPURequestAdapterCallback callback;
  for (int i = 0; i < 2; ++i) {
    if (args[i].get_type() == sol::type::function) {
      callback = args[i].as<sol::function>();
      break;
    }
  }
  gpu->RequestAdapter(gpu_options, std::move(callback));
}

}  // namespace seen::runtime::glue

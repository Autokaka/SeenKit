// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/22.

#pragma once

#include <sol/sol.hpp>

#include "seen/mod/gpu.h"

namespace seen::runtime::glue {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

void GPURequestAdapter(GPU* gpu, const sol::variadic_args& args) {
  auto gpu_options = GPU::RequestAdapterOptions::kDefault;
  if (args[0].get_type() == sol::type::table) {
    auto options = args[0].as<sol::table>();
    auto power_pref = options["powerPreference"];
    if (power_pref.get_type() == sol::type::string) {
      auto pref_str = power_pref.get<std::string>();
      if (GPUAdapter::IsPowerPref(pref_str)) {
        gpu_options.power_preference = pref_str.c_str();
      }
    }
  }
  gpu->RequestAdapter(gpu_options, [args](const GPUAdapter::Ptr& adapter) {
    for (int i = 0; i < 2; ++i) {
      if (args[i].get_type() == sol::type::function) {
        args[i].as<sol::function>().call(adapter);
        return;
      }
    }
  });
}

}  // namespace seen::runtime::glue

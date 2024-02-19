// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <sol/sol.hpp>

#include "seen/mod/gpu.h"

namespace seen::mod {

class Seen final {
 public:
  using Ptr = std::shared_ptr<Seen>;

  static void Log(const sol::variadic_args& args);

  static const char* GetVersion();

  static GPU::Ptr GetGPU();
};

}  // namespace seen::mod

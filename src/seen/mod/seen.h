// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <sol/sol.hpp>

#include "seen/mod/gpu.h"

namespace seen::mod {

class Seen final {
 public:
  explicit Seen();
  ~Seen();

  void Log(const sol::variadic_args& args) const;
  GPU::Ptr GetGPU();

  const char* version;

 private:
  GPU::Ptr gpu_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Seen);
};

}  // namespace seen::mod

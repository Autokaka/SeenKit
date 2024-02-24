// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/seen.h"
#include "seen/base/logger.h"

namespace seen::mod {

Seen::Seen() : version(SEEN_VERSION) {
  SEEN_INFO("Create Seen instance.");
}

Seen::~Seen() {
  SEEN_INFO("Release Seen instance.");
}

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
void Seen::Log(const sol::variadic_args& args) const {
  std::string message;
  for (auto&& arg : args) {
    std::string desc = luaL_tolstring(arg.lua_state(), arg.stack_index(), nullptr);
    message += desc + " ";
  }
  SEEN_INFO("{}", message);
}

GPU::Ptr Seen::GetGPU() {
  gpu_ || (gpu_ = GPU::Create());
  return gpu_;
}

}  // namespace seen::mod

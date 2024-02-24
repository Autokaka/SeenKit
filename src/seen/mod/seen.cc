// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/mod/seen.h"
#include "seen/base/logger.h"

namespace seen::mod {

Seen::Seen() : version(SEEN_VERSION) {}

void Seen::Log(const sol::variadic_args& args) const {
  auto tag = reinterpret_cast<std::uintptr_t>(this);
  std::string message;
  for (auto&& arg : args) {
    std::string desc = luaL_tolstring(arg.lua_state(), arg.stack_index(), nullptr);
    message += desc + " ";
  }
  SEEN_INFO("<{}> {}", tag, message);
}

GPU::Ptr Seen::GetGPU() {
  gpu_ || (gpu_ = GPU::Create());
  return gpu_;
}

}  // namespace seen::mod

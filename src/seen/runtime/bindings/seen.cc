// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/bindings/seen.h"
#include "seen/base/logger.h"

namespace seen::runtime {

void Seen::Log(const sol::variadic_args& args) {
  std::string message;
  for (auto&& arg : args) {
    std::string desc = luaL_tolstring(arg.lua_state(), arg.stack_index(), nullptr);
    message += desc + " ";
  }
  SEEN_INFO("{}", message);
}

const char* Seen::GetVersion() {
  return SEEN_VERSION;
}

GPU::Ptr Seen::GetGPU() {
  thread_local auto gpu = GPU::Create();
  return gpu;
}

}  // namespace seen::runtime

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/09.

#include "seen/runtime/export.h"
#include "seen/base/logger.h"

namespace seen::runtime {

namespace {

void TestLog(wasm_exec_env_t env, const char* message) {
  SEEN_INFO("{}", message);
}

template <typename Func>
NativeSymbol NS(const char* symbol, Func func_ptr, const char* sig, void* attachment = nullptr) {
  return {symbol, reinterpret_cast<void*>(func_ptr), sig, attachment};
}

}  // namespace

std::vector<NativeSymbol> ExportNativeSymbols() {
  return {
      NS("log", TestLog, "($)"),
  };
}

}  // namespace seen::runtime

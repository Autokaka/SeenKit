// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/09.

#include <wasm_export.h>
#include <vector>

#include "seen/base/logger.h"
#include "seen/runtime/register.h"

namespace seen::runtime {

namespace {

void TestLog(wasm_exec_env_t env, const char* message) {
  SEEN_INFO("{}", message);
}

template <typename Func>
NativeSymbol NS(const char* symbol, Func func_ptr, const char* sig, void* attachment = nullptr) {
  return {symbol, reinterpret_cast<void*>(func_ptr), sig, attachment};
}

std::vector<NativeSymbol> CreateNativeSymbols() {
  return {
      NS("log", TestLog, "($)"),
  };
}

}  // namespace

void RegisterNativeSymbols() {
  thread_local auto native_symbols = CreateNativeSymbols();
  auto* native_symbols_ptr = const_cast<NativeSymbol*>(native_symbols.data());
  wasm_runtime_register_natives("seen", native_symbols_ptr, native_symbols.size());
}

}  // namespace seen::runtime

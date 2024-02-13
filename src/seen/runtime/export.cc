// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/09.

#include <cstddef>

#include "seen/base/logger.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

namespace {

void WAMRLog(wasm_exec_env_t env, const char* message) {
  SEEN_INFO("{}", message);
}

std::size_t WAMRGetVersionByteLength(wasm_exec_env_t env) {
  return std::strlen(SEEN_VERSION);
}

void WAMRGetVersion(wasm_exec_env_t env, void* out_version, std::size_t len) {
  auto* module_instance = get_module_inst(env);
  std::memcpy(out_version, SEEN_VERSION, len);
}

template <typename Func>
inline NativeSymbol SymbolOf(const char* symbol, Func func_ptr, const char* sig, void* attachment = nullptr) {
  return {symbol, reinterpret_cast<void*>(func_ptr), sig, attachment};
}

}  // namespace

std::vector<NativeSymbol> ExportNativeSymbols() {
  return {
      SymbolOf("log", WAMRLog, "($)"),
      SymbolOf("get_version_byte_length", WAMRGetVersionByteLength, "()i"),
      SymbolOf("get_version", WAMRGetVersion, "(*~)"),
  };
}

}  // namespace seen::runtime

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/16.

#pragma once

#include <wasm_export.h>
#include <cstdint>

#include "seen/base/logger.h"

namespace seen::runtime {

enum class ValueType : std::uint8_t {
  kI32 = 0x7F,
  kI64 = 0x7E,
  kF32 = 0x7D,
  kF64 = 0x7C,
  kV128 = 0x7B,
  kFuncRef = 0x70,
  kExternRef = 0x6F,
  kVoid = 0x40,
};

void wasm_runtime_call_indirect_a(wasm_exec_env_t exec_env,
                                  std::uint32_t function_id,
                                  std::uint32_t result_count,
                                  const ValueType* result_types,
                                  std::uint32_t arg_count,
                                  const wasm_val_t* args,
                                  wasm_val_t* out_results);

void wasm_runtime_call_indirect_a(wasm_exec_env_t exec_env,
                                  std::uint32_t function_id,
                                  std::uint32_t arg_count,
                                  const wasm_val_t* args);

template <typename T>
inline wasm_val_t wasm_ptr_val(T* value) {
  if constexpr (sizeof(std::uintptr_t) == sizeof(std::int32_t)) {
    return {.kind = WASM_I32, .__paddings = {0}, .of = {.i32 = reinterpret_cast<std::int32_t>(value)}};
  } else if constexpr (sizeof(std::uintptr_t) == sizeof(std::int64_t)) {
    return {.kind = WASM_I64, .__paddings = {0}, .of = {.i64 = reinterpret_cast<std::int64_t>(value)}};
  }
  SEEN_ASSERT_WITH_MESSAGE(false, "Unsupported pointer type");
}

template <typename T>
inline wasm_val_t wasm_externref_val(T* ptr) {
  return {.kind = WASM_ANYREF, .__paddings = {0}, .of = {.foreign = reinterpret_cast<std::uintptr_t>(ptr)}};
}

template <typename T>
inline wasm_val_t wasm_i32_val(T value) {
  return {.kind = WASM_I32, .__paddings = {0}, .of = {.i32 = static_cast<std::int32_t>(value)}};
}

template <typename T>
inline wasm_val_t wasm_i64_val(T value) {
  return {.kind = WASM_I64, .__paddings = {0}, .of = {.i64 = static_cast<std::int64_t>(value)}};
}

template <typename T>
inline wasm_val_t wasm_f32_val(T value) {
  return {.kind = WASM_F32, .__paddings = {0}, .of = {.f32 = static_cast<float>(value)}};
}

template <typename T>
inline wasm_val_t wasm_f64_val(T value) {
  return {.kind = WASM_F64, .__paddings = {0}, .of = {.f64 = static_cast<double>(value)}};
}

inline wasm_val_t wasm_init_val() {
  return {.kind = WASM_ANYREF, .__paddings = {0}, .of = {.ref = nullptr}};
}

template <auto FuncPtr>
inline constexpr NativeSymbol wasm_symbol(const char* name, const char* sig, void* attachment = nullptr) {
  return {name, reinterpret_cast<void*>(FuncPtr), sig, attachment};
}

}  // namespace seen::runtime

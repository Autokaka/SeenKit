// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#pragma once

#include <wasm_c_api.h>
#include <cstdint>
#include <utility>

#include "seen/base/logger.h"

namespace seen::runtime {

template <typename T>
wasm_val_t ToWASMValue(T value) {
  // TODO(Autokaka):
}

template <typename T>
T ToNativeValue(wasm_val_t value) {
  // TODO(Autokaka):
  if constexpr (std::is_pointer_v<T> || sizeof(T) == sizeof(std::int32_t)) {
    return reinterpret_cast<T>(value.of.i32);
  }
  SEEN_ASSERT_WITH_MESSAGE(false, "Unbound type!");
}

template <typename T>
wasm_valtype_t* ToWASMType() {
  // TODO(Autokaka):
  if constexpr (std::is_pointer_v<T> || sizeof(T) == sizeof(std::int32_t)) {
    return wasm_valtype_new_i32();
  }
  SEEN_ASSERT_WITH_MESSAGE(false, "Unbound type!");
}

template <typename... Types>
void ToWASMTypes(wasm_valtype_vec_t* out_types) {
  constexpr auto type_count = sizeof...(Types);
  wasm_valtype_t* types[type_count] = {ToWASMType<Types>()...};
  wasm_valtype_vec_new(out_types, type_count, types);
}

template <>
inline void ToWASMTypes<void>(wasm_valtype_vec_t* out_types) {
  wasm_valtype_vec_new_empty(out_types);
}

template <auto CFunc, typename ReturnType, typename... Args, std::size_t... I>
wasm_func_callback_t ToWASMFunction(ReturnType (*_c_fn)(Args...), std::index_sequence<I...> _seq) {
  return [](const wasm_val_vec_t* args, wasm_val_vec_t* results) -> wasm_trap_t* {
    if constexpr (std::is_void_v<ReturnType>) {
      CFunc(ToNativeValue<Args>(args->data[I])...);
    } else {
      auto result = CFunc(ToNativeValue<Args>(args->data[I])...);
      results->data[0] = ToWASMValue<ReturnType>(result);
    }
    return nullptr;
  };
}

template <auto CFunc, typename ReturnType, typename... Args>
wasm_func_callback_t ToWASMFunction(ReturnType (*c_fn)(Args...)) {
  return ToWASMFunction<CFunc>(c_fn, std::index_sequence_for<Args...>());
}

template <auto CFunc>
wasm_func_callback_t ToWASMFunction() {
  return ToWASMFunction<CFunc>(CFunc);
}

}  // namespace seen::runtime

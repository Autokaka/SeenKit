// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#pragma once

#include <wasm_c_api.h>
#include <utility>

namespace seen::runtime {

template <typename T>
wasm_val_t ToWASMValue(T value) {
  // TODO(Autokaka):
}

template <typename T>
T ToNativeValue(wasm_val_t value) {
  // TOOO(Autokaka):
  return nullptr;
}

template <typename T>
wasm_valtype_t* ToWASMType() {
  // TODO(Autokaka):
  // wasm_valtype_new(wasm_valkind_enum::WASM_I32);
  return nullptr;
}

template <typename... Types>
void ToWASMTypes(wasm_valtype_vec_t* out) {
  const auto type_count = sizeof...(Types);
  wasm_valtype_t* types[type_count] = {ToWASMType<Types>()...};
  wasm_valtype_vec_new(out, type_count, types);
}

template <>
inline void ToWASMTypes<void>(wasm_valtype_vec_t* out) {
  wasm_valtype_vec_new_empty(out);
}

template <auto CFunc, typename ReturnType, typename... Args, std::size_t... I>
wasm_func_callback_t ToWASMFunction(ReturnType (*_c_fn)(Args...), std::index_sequence<I...> _seq) {
  return [](const wasm_val_vec_t* args, wasm_val_vec_t* results) -> wasm_trap_t* {
    if constexpr (std::is_void_v<ReturnType>) {
      CFunc(ToNativeValue<Args>(args->data[I])...);
    } else {
      auto result = CFunc(ToNativeValue<Args>(args->data[I])...);
      results->data[0] = ToWASMValue(result);
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

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#pragma once

#include <wasm_c_api.h>
#include <string>
#include <unordered_map>

#include "seen/runtime/converter.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/module.h"
#include "seen/runtime/utils.h"

namespace seen::runtime {

using FunctionType = std::shared_ptr<wasm_functype_t>;
using Function = std::shared_ptr<wasm_func_t>;
using BindingMap = std::unordered_map<std::string, Function>;
using ExportVector = std::shared_ptr<wasm_extern_vec_t>;

extern thread_local BindingMap tls_binding_map;

template <auto CFunc, typename ReturnType, typename... Args>
void Bind(const char* name, ReturnType (*_c_fn)(Args...)) {
  auto store = GetTLSStore();
  wasm_valtype_vec_t params;
  ToWASMTypes<Args...>(&params);
  wasm_valtype_vec_t results;
  ToWASMTypes<ReturnType>(&results);
  auto* fn_type_ptr = wasm_functype_new(&params, &results);
  auto fn_type = FunctionType(fn_type_ptr, wasm_functype_delete);
  auto* bind_fn_ptr = wasm_func_new(store.get(), fn_type.get(), ToWASMFunction<CFunc>());
  tls_binding_map[name] = Function(bind_fn_ptr, wasm_func_delete);
}

template <auto OrigFn, typename ClassType, typename ReturnType, typename... Args>
void Bind(const char* name, ReturnType (ClassType::*_orig_fn)(Args...) const) {
  using CFunc = utils::CFunction<OrigFn>;
  // clang-format off
  Bind<CFunc::template Apply<ClassType, ReturnType, Args...>, ReturnType, Args...>(name, 
       CFunc::template Apply<ClassType, ReturnType, Args...>);
  // clang-format on
}

template <auto OrigFn, typename ClassType, typename ReturnType, typename... Args>
void Bind(const char* name, ReturnType (ClassType::*_orig_fn)(Args...)) {
  using CFunc = utils::CFunction<OrigFn>;
  // clang-format off
  Bind<CFunc::template Apply<ClassType, ReturnType, Args...>, ReturnType, Args...>(name, 
       CFunc::template Apply<ClassType, ReturnType, Args...>);
  // clang-format on
}

template <auto OrigFn>
void Bind(const char* name) {
  Bind<OrigFn>(name, OrigFn);
}

ExportVector LinkModule(const ModulePtr& wasm_module);

}  // namespace seen::runtime

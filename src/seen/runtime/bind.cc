// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#include <vector>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/bind.h"

namespace seen::runtime {

const char* const kSeenModuleName = "seen";
thread_local BindingMap tls_binding_map;

// https://github.com/bytecodealliance/wasm-micro-runtime/blob/main/samples/wasm-c-api-imports/README.md
ExportVector LinkModule(const ModulePtr& wasm_module) {
  SEEN_INFO("Link module on: {}.", CFWorker::GetCurrent()->GetName());
  auto* imports_ptr = new wasm_importtype_vec_t({0});
  auto imports = std::shared_ptr<wasm_importtype_vec_t>(imports_ptr, wasm_importtype_vec_delete);
  wasm_module_imports(wasm_module.get(), imports_ptr);

  std::vector<wasm_extern_t*> externs;
  externs.reserve(imports->num_elems);
  auto store = GetTLSStore();
  for (int i = 0; i < imports->num_elems; i++) {
    wasm_importtype_t* import_type = imports->data[i];
    if (wasm_importtype_is_linked(import_type)) {
      auto* placeholder = wasm_extern_new_empty(store.get(), wasm_externtype_kind(wasm_importtype_type(import_type)));
      externs.emplace_back(placeholder);
      continue;
    }
    const auto* module_name = wasm_importtype_module(imports->data[i]);
    std::string module_name_str = module_name->data;
    if (module_name_str == kSeenModuleName) {
      const auto* field_name = wasm_importtype_name(imports->data[i]);
      std::string field_name_str = field_name->data;
      auto bind_fn = tls_binding_map[field_name_str];
      if (bind_fn) {
        externs.emplace_back(wasm_func_as_extern(bind_fn.get()));
      }
    }
  }

  auto* exports_ptr = new wasm_extern_vec_t({0});
  wasm_extern_vec_new(exports_ptr, imports->num_elems, externs.data());
  return {exports_ptr, wasm_extern_vec_delete};
}

}  // namespace seen::runtime

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#include <fstream>

#include "seen/base/deferred_task.h"
#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/module.h"

namespace seen::runtime {

ModulePtr CreateModule(const fs::path& path) {
  auto worker_name = CFWorker::GetCurrent()->GetName();
  SEEN_INFO("Create wasm_module on: {}.", worker_name);
  std::ifstream ifs(path, std::ios::binary);
  CFDeferredTask defer_ifs([&ifs]() { ifs.close(); });
  if (!ifs) {
    SEEN_ERROR("Failed to create wasm_module on: {}. File not exists!", worker_name);
    return nullptr;
  }
  auto store = GetTLSStore();
  ifs.seekg(0, std::ifstream::end);
  auto length = ifs.tellg();
  ifs.seekg(0, std::ifstream::beg);
  wasm_byte_vec_t bytes;
  wasm_byte_vec_new_uninitialized(&bytes, length);
  CFDeferredTask defer([&bytes]() { wasm_byte_vec_delete(&bytes); });
  ifs.read(bytes.data, length);
  if (!ifs) {
    SEEN_ERROR("Failed to create wasm_module on: {}. Failed to write bytes!", worker_name);
    return nullptr;
  }
  if (!wasm_module_validate(store.get(), &bytes)) {
    SEEN_ERROR("Failed to create wasm_module on: {}. Invalid module!", worker_name);
    return nullptr;
  }
  return {wasm_module_new(store.get(), &bytes), wasm_module_delete};
}

}  // namespace seen::runtime

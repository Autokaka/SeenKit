#include <memory>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/bind.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/eval.h"

namespace seen::runtime {

void EvaluateModule(const ModulePtr& module) {
  auto worker_name = CFWorker::GetCurrent()->GetName();
  SEEN_INFO("Evaluate module on: {}.", worker_name);
  auto store = GetTLSStore();
  auto exports = LinkModule(module);
  auto* instance_ptr = wasm_instance_new(store.get(), module.get(), &*exports, nullptr);
  if (instance_ptr == nullptr) {
    SEEN_INFO("Failed to evaluate module on: {}.", worker_name);
    return;
  }
  auto instance = std::shared_ptr<wasm_instance_t>(instance_ptr, wasm_instance_delete);
}

}  // namespace seen::runtime

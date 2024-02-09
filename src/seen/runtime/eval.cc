#include <memory>

#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/bind.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/eval.h"
#include "seen/runtime/register.h"

namespace seen::runtime {

void EvaluateModule(const ModulePtr& module) {
  auto worker_name = CFWorker::GetCurrent()->GetName();
  SEEN_INFO("Evaluate module on: {}.", worker_name);
  RegisterBindings();
  auto store = GetTLSStore();
  auto bindings = LinkModule(module);
  auto* instance_ptr = wasm_instance_new(store.get(), module.get(), bindings.get(), nullptr);
  if (instance_ptr == nullptr) {
    SEEN_INFO("Failed to evaluate module on: {}.", worker_name);
    return;
  }
  auto instance = std::shared_ptr<wasm_instance_t>(instance_ptr, wasm_instance_delete);
  auto exports = ExportVector(new wasm_extern_vec_t, wasm_extern_vec_delete);
  wasm_instance_exports(instance.get(), exports.get());
  SEEN_ASSERT_WITH_MESSAGE(exports->size > 0, "Module exports is empty!");
  const auto* start_func = wasm_extern_as_func(exports->data[1]);
  SEEN_ASSERT_WITH_MESSAGE(start_func, "Module should always have a start function!");

  wasm_val_t result_types[1] = {WASM_I32_VAL(0)};
  wasm_val_vec_t args = WASM_EMPTY_VEC;
  wasm_val_vec_t results = WASM_ARRAY_VEC(result_types);
  wasm_trap_t* trap_ptr = wasm_func_call(start_func, &args, &results);
  auto trap = std::shared_ptr<wasm_trap_t>(trap_ptr, wasm_trap_delete);
  if (trap) {
    auto message = std::shared_ptr<wasm_name_t>(new wasm_name_t{0}, wasm_name_delete);
    wasm_trap_message(trap.get(), message.get());
    SEEN_ERROR("Error: {}", message->data);
  }
}

}  // namespace seen::runtime

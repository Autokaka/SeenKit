// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/07.

#pragma once

#include <wasm_c_api.h>

#include "seen/runtime/module.h"

namespace seen::runtime {

void EvaluateModule(const ModulePtr& module);

}  // namespace seen::runtime

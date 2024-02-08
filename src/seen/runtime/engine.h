// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#include <wasm_c_api.h>
#include <memory>

#pragma once

namespace seen::runtime {

using EnginePtr = std::shared_ptr<wasm_engine_t>;
using StorePtr = std::shared_ptr<wasm_store_t>;

EnginePtr GetEngine();

StorePtr GetTLSStore();

}  // namespace seen::runtime

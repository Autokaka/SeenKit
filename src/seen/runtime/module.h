// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#pragma once

#include <wasm_c_api.h>
#include <filesystem>
#include <memory>

namespace seen::runtime {

namespace fs = std::filesystem;
using ModulePtr = std::shared_ptr<wasm_module_t>;

ModulePtr CreateModule(const fs::path& path);

}  // namespace seen::runtime

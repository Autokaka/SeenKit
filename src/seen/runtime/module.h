// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#pragma once

#include <wasm_export.h>
#include <filesystem>
#include <memory>

namespace seen::runtime {

namespace fs = std::filesystem;
using Module = std::shared_ptr<WASMModuleCommon>;

Module LoadModule(const fs::path& path);

}  // namespace seen::runtime

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <filesystem>

namespace seen::runtime {

namespace fs = std::filesystem;

bool ExecEntry(const fs::path& entry_file);

}  // namespace seen::runtime

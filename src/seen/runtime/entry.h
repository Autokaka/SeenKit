// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <filesystem>

#include "seen/mod/seen.h"

namespace seen::runtime {

namespace fs = std::filesystem;

mod::Seen::Ptr ExecEntry(const fs::path& entry_file);

}  // namespace seen::runtime

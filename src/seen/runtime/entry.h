// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <filesystem>
#include <memory>
#include <sol/sol.hpp>

#include "seen/base/worker.h"

namespace seen::runtime {

namespace fs = std::filesystem;
using StatePtr = std::shared_ptr<sol::state>;

StatePtr ExecEntry(const CFWorker::Ptr& runner, const fs::path& entry_file);

}  // namespace seen::runtime

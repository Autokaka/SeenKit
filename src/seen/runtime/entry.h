// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <filesystem>
#include <memory>
#include <sol/sol.hpp>

#include "seen/base/worker.h"

namespace seen::runtime {

namespace fs = std::filesystem;
using StatePtr = std::shared_ptr<sol::state>;

StatePtr ExecEntry(const Worker::Ptr& runner, const fs::path& entry_file);

}  // namespace seen::runtime

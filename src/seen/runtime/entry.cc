// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include <sol/sol.hpp>

#include "seen/base/data.h"
#include "seen/base/logger.h"
#include "seen/runtime/entry.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

void ExecEntry(const fs::path& entry_file) {
  sol::state lua;
  lua.open_libraries();
  ExportHostAbilities(&lua);
  auto data = CFData::CreateFromAbsolutePath(entry_file);
  std::string script = reinterpret_cast<char*>(data->GetBytes());
  try {
    fs::current_path(entry_file.parent_path());
    lua.script_file(entry_file);
  } catch (const sol::error& e) {
    SEEN_ERROR("{}", e.what());
  }
}

}  // namespace seen::runtime

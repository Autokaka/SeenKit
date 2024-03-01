// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/entry.h"
#include "seen/base/data.h"
#include "seen/base/logger.h"
#include "seen/mod/seen.h"
#include "seen/runtime/export.h"

namespace seen::runtime {

StatePtr ExecEntry(const Worker::Ptr& runner, const fs::path& entry_file) {
  auto lua = std::make_shared<sol::state>();
  lua->open_libraries();
  ExportModules(lua.get());
  auto data = Data::CreateFromAbsolutePath(entry_file);
  std::string script = reinterpret_cast<char*>(data->GetBytes());
  try {
    fs::current_path(entry_file.parent_path());
    auto global = lua->globals();
    global["seen"] = std::make_shared<mod::Seen>(runner);
    lua->script_file(entry_file);
  } catch (const sol::error& e) {
    SEEN_ERROR("{}", e.what());
    return nullptr;
  }
  lua->collect_gc();
  return lua;
}

}  // namespace seen::runtime

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include <ScriptX/ScriptX.h>

#include "seen/base/data.h"
#include "seen/base/logger.h"
#include "seen/runtime/entry.h"
#include "seen/runtime/export.h"
#include "seen/runtime/types.h"

namespace seen::runtime {

void ExecEntry(const fs::path& entry_file) {
  thread_local auto tls_engine = EnginePtr(new script::ScriptEngineImpl(), script::ScriptEngine::Deleter());
  script::EngineScope enter(tls_engine.get());
  ExportHostAbilities(tls_engine);
  auto data = CFData::CreateFromAbsolutePath(entry_file);
  std::string script = reinterpret_cast<char*>(data->GetBytes());
  try {
    fs::current_path(entry_file.parent_path());
    tls_engine->eval(script, entry_file.filename());
  } catch (const script::Exception& e) {
    SEEN_ERROR("{}", e.what());
  }
}

}  // namespace seen::runtime

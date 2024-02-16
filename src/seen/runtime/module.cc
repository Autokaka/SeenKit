// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#include <cstdint>

#include "seen/base/data.h"
#include "seen/base/logger.h"
#include "seen/base/worker.h"
#include "seen/runtime/engine.h"
#include "seen/runtime/module.h"

namespace seen::runtime {

static constexpr auto kErrorMaxLength = 256;

Module LoadModule(const fs::path& path) {
  static constexpr char const* tag = "LoadModule";
  static auto engine = std::make_unique<Engine>();
  if (!engine) {
    SEEN_ERROR("{} failed: engine is not initialized!", tag);
    return nullptr;
  }

  SEEN_INFO("{} on {}.", tag, CFWorker::GetCurrent()->GetName());
  auto data = CFData::CreateFromAbsolutePath(path);
  if (!data) {
    SEEN_ERROR("{} failed: path '{}' does not exist!", tag, path.c_str());
    return nullptr;
  }

  char error_buf[kErrorMaxLength];
  auto* buf = reinterpret_cast<std::uint8_t*>(data->GetBytes());
  auto buf_size = static_cast<std::uint32_t>(data->GetSize());
  auto* module_ptr = wasm_runtime_load(buf, buf_size, error_buf, sizeof(error_buf));
  if (module_ptr == nullptr) {
    SEEN_ERROR("{} failed: {}.", tag, error_buf);
    return nullptr;
  }

  return {module_ptr, wasm_runtime_unload};
}

}  // namespace seen::runtime

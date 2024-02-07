#include <miniz.h>

#include "seen/base/deferred_task.h"
#include "seen/base/time_point.h"
#include "seen/bundle.h"

namespace seen {

namespace {

bool Unzip(const fs::path& absolute_path, const fs::path& output_dir) {
  auto assets_path = output_dir / Bundle::kAssetsDirName;
  fs::create_directories(assets_path);
  mz_zip_archive zip;
  std::memset(&zip, 0, sizeof(zip));
  if (mz_zip_reader_init_file(&zip, absolute_path.c_str(), 0) != MZ_TRUE) {
    return false;
  }
  CFDeferredTask defer_zip([&zip]() { mz_zip_reader_end(&zip); });
  auto file_count = mz_zip_reader_get_num_files(&zip);
  bool entry_extracted = false;
  for (int i = 0; i < file_count; i += 1) {
    mz_zip_archive_file_stat file_stat;
    if (mz_zip_reader_file_stat(&zip, i, &file_stat) != MZ_TRUE) {
      return false;
    }
    auto is_dir = mz_zip_reader_is_file_a_directory(&zip, i) == MZ_TRUE;
    auto* file_name = file_stat.m_filename;
    auto is_entry = !is_dir && std::strcmp(file_name, Bundle::kEntryFileName) == 0;
    auto is_assets_file = std::strncmp(file_name, Bundle::kAssetsDirName, std::strlen(Bundle::kAssetsDirName)) == 0;
    const char* wasm_file_suffix = ".wasm";
    const char* file_suffix = file_name + std::strlen(file_name) - std::strlen(wasm_file_suffix);
    auto is_wasm = !is_dir && std::strcmp(file_suffix, ".wasm") == 0;
    auto should_extract = is_entry || is_assets_file || is_wasm;
    if (!should_extract) {
      continue;
    }
    auto out_path = output_dir / file_name;
    fs::create_directories(out_path.parent_path());
    if (is_dir) {
      fs::create_directories(out_path);
    } else if (mz_zip_reader_extract_to_file(&zip, i, out_path.c_str(), 0) != MZ_TRUE) {
      return false;
    }
    entry_extracted = entry_extracted || is_entry;
  }
  return entry_extracted;
}

}  // namespace

const char* Bundle::kEntryFileName = "main.wasm";
const char* Bundle::kAssetsDirName = "assets/";

Bundle::Ptr Bundle::CreateFromPath(const fs::path& absolute_path) {
  auto bundle = std::make_shared<Bundle>(absolute_path);
  return bundle->is_valid_ ? bundle : nullptr;
}

Bundle::Bundle(const fs::path& absolute_path) : is_valid_(false) {
  auto temp_dir = fs::temp_directory_path();
  do {
    auto nanos = std::to_string(TimePoint::Now().ToEpochDelta().ToNanoseconds());
    bundle_dir_ = temp_dir / "seen" / nanos;
  } while (fs::exists(bundle_dir_));
  if (!fs::create_directories(bundle_dir_)) {
    return;
  }
  if (!Unzip(absolute_path, bundle_dir_)) {
    return;
  }
  is_valid_ = true;
}

Bundle::~Bundle() {
  fs::remove_all(bundle_dir_);
}

fs::path Bundle::GetEntryPath() const {
  return bundle_dir_ / kEntryFileName;
}

fs::path Bundle::GetAssetsDirectory() const {
  return bundle_dir_ / kAssetsDirName;
}

}  // namespace seen

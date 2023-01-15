/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#pragma once

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

namespace seen::CFFS {

namespace fs = std::filesystem;

fs::path GetTempDirectory();

bool Unzip(const fs::path& zip_absolute_path, const fs::path& out_directory);

}  // namespace seen::CFFS

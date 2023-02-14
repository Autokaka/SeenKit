/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/12.
 */

#pragma once

#include <filesystem>
#include <string>

namespace seen::CFFS {

namespace stdfs = std::filesystem;

stdfs::path GetTempDirectory();

bool Unzip(const stdfs::path& zip_absolute_path, const stdfs::path& out_directory);

}  // namespace seen::CFFS

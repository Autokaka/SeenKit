// Created by Autokaka (qq1909698494@gmail.com) on 2023/01/15.

#pragma once

#include <filesystem>
#include <string>

namespace seen::CFFS {

namespace stdfs = std::filesystem;

bool Unzip(const stdfs::path& zip_absolute_path, const stdfs::path& out_directory);

stdfs::path GetTempDirectory();  // Implemented on each platform.

}  // namespace seen::CFFS

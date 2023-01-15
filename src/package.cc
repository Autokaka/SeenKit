/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <chrono>
#include <filesystem>
#include <string>
#include <utility>

#include "foundation/file_system.h"
#include "package.h"

namespace seen {

namespace {

constexpr int kPkgMaxRetryTimes = 1024;

}

std::unique_ptr<Package> Package::CreateFromFile(const std::string& absolute_path) {
  if (!fs::is_regular_file(absolute_path)) {
    return nullptr;
  }

  auto now = std::chrono::system_clock::now();
  auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now).time_since_epoch().count();
  auto pkg_tmp_dir_name = "pkg_" + std::to_string(now_ms);
  auto pkg_tmp_dir = CFFS::GetTempDirectory() / pkg_tmp_dir_name;
  bool pkg_tmp_dir_created = false;
  static std::mutex mutex;
  {
    std::scoped_lock lock(mutex);
    for (auto i = 1; i <= kPkgMaxRetryTimes; ++i) {
      auto try_path = pkg_tmp_dir;
      if (i > 1) {
        try_path += "_" + std::to_string(i);
      }
      if (fs::is_empty(try_path)) {
        pkg_tmp_dir_created = fs::create_directory(try_path);
        pkg_tmp_dir = try_path;
        break;
      }
    }
  }
  if (!pkg_tmp_dir_created) {
    return nullptr;
  }

  if (!CFFS::Unzip(absolute_path, pkg_tmp_dir)) {
    return nullptr;
  }

  // TODO(Autokaka): Check package validity.

  return nullptr;
}

Package::Package(Info info, CFData::Ptr module, std::string resource_directory, std::string sandbox_directory)
    : info_(std::move(info)),
      module_(std::move(module)),
      resource_directory_(std::move(resource_directory)),
      sandbox_directory_(std::move(sandbox_directory)) {}

}  // namespace seen

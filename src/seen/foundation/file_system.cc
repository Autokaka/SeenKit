/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/02/14.
 */

#include <miniunz/unzip.h>
#include <fstream>

#include "seen/foundation/file_system.h"

namespace seen::CFFS {

namespace {

constexpr uInt kWriteBufferSize = 1024 * 8;

bool ExtractFile(unzFile file, const stdfs::path& out_directory) {
  char path_buffer[PATH_MAX];
  unz_file_info file_info;
  if (unzGetCurrentFileInfo(file, &file_info, path_buffer, sizeof(path_buffer), nullptr, 0, nullptr, 0) != UNZ_OK) {
    return false;
  }

  std::string path_in_zip(path_buffer);
  if (path_in_zip.empty()) {
    return false;
  }
  auto file_absolute_path = out_directory / path_in_zip;
  bool is_directory = (path_in_zip[path_in_zip.size() - 1] == '/');
  if (is_directory) {
    return stdfs::create_directory(file_absolute_path);
  }

  if (unzOpenCurrentFile(file) != UNZ_OK) {
    return false;
  }

  struct ScopedCurrentUnzFile {
    explicit ScopedCurrentUnzFile(unzFile file) : file_(file) {}
    ~ScopedCurrentUnzFile() { unzCloseCurrentFile(file_); }

   private:
    unzFile file_;
  } scoped_current_unz_file(file);

  std::ofstream ofs(file_absolute_path, std::ios::out | std::ios::binary);
  if (!ofs) {
    auto directory_name = file_absolute_path.parent_path();
    if (!directory_name.empty()) {
      stdfs::create_directory(directory_name);
      ofs.open(file_absolute_path, std::ios::out | std::ios::binary);
      if (!ofs) {
        return false;
      }
    }
  }

  auto buffer = std::make_unique<char[]>(kWriteBufferSize);
  int count = 0;
  while ((count = unzReadCurrentFile(file, static_cast<void*>(buffer.get()), kWriteBufferSize)) > 0) {
    if (ofs.write(buffer.get(), count).bad()) {
      return false;
    }
  }
  return count >= 0;
}

}  // namespace

bool Unzip(const stdfs::path& zip_absolute_path, const stdfs::path& out_directory) {
  unzFile file = unzOpen(zip_absolute_path.c_str());
  if (file == nullptr) {
    return false;
  }

  struct ScopedUnzFile {
    explicit ScopedUnzFile(unzFile file) : file_(file) {}
    ~ScopedUnzFile() { unzClose(file_); }

   private:
    unzFile file_;
  } scoped_unz_file(file);

  unz_global_info global_info;
  if (unzGetGlobalInfo(file, &global_info) == UNZ_OK) {
    for (uLong i = 0; i < global_info.number_entry; i++) {
      if (!ExtractFile(file, out_directory)) {
        return false;
      }
      if (i + 1 < global_info.number_entry && unzGoToNextFile(file) != UNZ_OK) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace seen::CFFS

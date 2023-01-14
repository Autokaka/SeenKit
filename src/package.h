/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <unordered_map>

#include "foundation/promise.h"

namespace seen {

class Package {
 public:
  using Info = std::unordered_map<std::string, std::string>;
  using ByteArray = std::vector<std::byte>;

  static std::unique_ptr<Package> CreateFromFile(const std::string& path);

  [[nodiscard]] Info GetInfo() const { return info_; }
  [[nodiscard]] std::string GetResourceDirectory() const { return resource_directory_; }
  [[nodiscard]] std::string GetSandboxDirectory() const { return sandbox_directory_; }
  [[nodiscard]] ByteArray GetModule() const { return module_; }

  CFPromise<bool> SaveFileToSandbox(const ByteArray& bytes, const std::string& file_path);
  CFPromise<ByteArray> GetFileFromSandbox(const std::string& file_path);

 private:
  Info info_;
  std::string resource_directory_;
  std::string sandbox_directory_;
  ByteArray module_;
};

}  // namespace seen

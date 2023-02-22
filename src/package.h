/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <unordered_map>

#include "seen/foundation/class_constraints.h"
#include "seen/foundation/data.h"
#include "seen/foundation/promise.h"

namespace seen {

class Package final {
 public:
  using Info = std::unordered_map<std::string, std::string>;
  using ByteArray = std::vector<std::byte>;

  static std::unique_ptr<Package> CreateFromFile(const std::string& absolute_path);
  Package(Info info, CFData::Ptr module, std::string resource_directory, std::string sandbox_directory);

  [[nodiscard]] Info GetInfo() const { return info_; }
  [[nodiscard]] std::string GetResourceDirectory() const { return resource_directory_; }
  [[nodiscard]] std::string GetSandboxDirectory() const { return sandbox_directory_; }
  [[nodiscard]] CFData::Ptr GetModuleCopy() const { return module_->Copy(); }

  CFPromise<bool> SaveFileToSandbox(const ByteArray& bytes, const std::string& file_path);
  CFPromise<CFData::Ptr> GetFileFromSandbox(const std::string& file_path);

 private:
  Info info_;
  CFData::Ptr module_;
  std::string resource_directory_;
  std::string sandbox_directory_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(Package);
};

}  // namespace seen

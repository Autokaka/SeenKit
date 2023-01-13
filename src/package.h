/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <cstddef>
#include <string>
#include <unordered_map>

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

  void SaveFileToSandbox(const ByteArray& bytes,
                         const std::string& file_path,
                         const std::function<void(bool)>& on_complete = nullptr);
  void GetFileFromSandbox(const std::string& file_path, const std::function<void(ByteArray)>& on_complete = nullptr);

 private:
  Info info_;
  std::string resource_directory_;
  std::string sandbox_directory_;
  ByteArray module_;
};

}  // namespace seen

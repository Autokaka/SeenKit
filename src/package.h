/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

namespace seen {

class Package {
 public:
  using Info = std::unordered_map<std::string, std::string>;

  static std::unique_ptr<Package> CreateFromFile(const std::string& path);

  [[nodiscard]] Info GetInfo() const { return info_; }
  [[nodiscard]] std::string GetResourceDirectory() const { return resource_directory_; }
  [[nodiscard]] std::vector<std::byte> GetModule() const { return module_; }

 private:
  Info info_;
  std::string resource_directory_;
  std::vector<std::byte> module_;
};

}  // namespace seen

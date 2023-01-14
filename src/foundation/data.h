/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/15.
 */

#include <cstddef>
#include <memory>
#include <vector>

namespace seen {

class CFData {
 public:
  using Ptr = std::shared_ptr<CFData>;

  static Ptr CreateFromAbsolutePath(const std::string& absolute_path);
  static Ptr CreateFromBytes(const std::byte* bytes, std::size_t size);
  static Ptr CreateFromBytesNoCopy(std::byte* bytes, std::size_t size);
  static Ptr CreateFromSize(std::size_t size);
  explicit CFData(std::byte* bytes, std::size_t size);

  [[nodiscard]] std::byte* GetBytes() const { return bytes_; }
  [[nodiscard]] std::size_t GetSize() const { return size_; }

 private:
  std::size_t size_;
  std::byte* bytes_;
};

}  // namespace seen

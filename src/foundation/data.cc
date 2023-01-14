/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/15.
 */

#include "data.h"

#include <fstream>
#include <string>

namespace seen {

CFData::Ptr CFData::CreateFromAbsolutePath(const std::string& absolute_path) {
  std::ifstream ifs(absolute_path, std::ios::binary);
  if (!ifs) {
    ifs.close();
    return nullptr;
  }

  ifs.seekg(0, ifs.end);
  auto length = ifs.tellg();
  ifs.seekg(0, ifs.beg);

  auto* buffer = new char[length];
  ifs.read(buffer, length);
  if (!ifs) {
    delete[] buffer;
    buffer = nullptr;
  }
  ifs.close();

  auto* bytes = reinterpret_cast<std::byte*>(buffer);
  return bytes == nullptr ? nullptr : CreateFromBytesNoCopy(bytes, length);
}

CFData::Ptr CFData::CreateFromBytes(const std::byte* bytes, std::size_t size) {
  auto* bytes_copy = new std::byte[size];
  std::memcpy(bytes_copy, bytes, size);
  return CreateFromBytesNoCopy(bytes_copy, size);
}

CFData::Ptr CFData::CreateFromBytesNoCopy(std::byte* bytes, std::size_t size) {
  return std::make_shared<CFData>(bytes, size);
}

CFData::Ptr CFData::CreateFromSize(std::size_t size) {
  return CreateFromBytesNoCopy(new std::byte[size], size);
}

CFData::CFData(std::byte* bytes, std::size_t size) : bytes_(bytes), size_(size) {}

}  // namespace seen

// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <cstddef>
#include <memory>

#include "seen/base/class_ext.h"

namespace seen {

class Data {
 public:
  using Ptr = std::shared_ptr<Data>;

  static Ptr CreateFromAbsolutePath(const std::string& absolute_path);
  static Ptr CreateFromBytes(const std::byte* bytes, std::size_t size);
  static Ptr CreateFromBytesNoCopy(std::byte* bytes, std::size_t size);
  static Ptr CreateFromSize(std::size_t size);
  explicit Data(std::byte* bytes, std::size_t size);
  ~Data();

  [[nodiscard]] std::byte* GetBytes() const { return bytes_; }
  [[nodiscard]] std::size_t GetSize() const { return size_; }
  [[nodiscard]] Ptr Copy() const;
  [[nodiscard]] Ptr Move();

 private:
  std::size_t size_;
  std::byte* bytes_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Data);
};

}  // namespace seen

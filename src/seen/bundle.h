// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <filesystem>

#include "seen/base/class_ext.h"

namespace seen {

namespace fs = std::filesystem;

class Bundle final {
 public:
  static const char* kEntryFileName;
  static const char* kAssetsDirName;
  using Ptr = std::shared_ptr<Bundle>;

  static Ptr CreateFromPath(const fs::path& absolute_path);
  explicit Bundle(const fs::path& absolute_path);
  ~Bundle();

  [[nodiscard]] fs::path GetEntryPath() const;
  [[nodiscard]] fs::path GetAssetsDirectory() const;

 private:
  fs::path bundle_dir_;
  bool is_valid_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Bundle);
};

}  // namespace seen

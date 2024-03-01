// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <wgpu/wgpu.h>
#include <memory>

#include "seen/base/class_ext.h"
#include "seen/base/rx_value.h"
#include "seen/mod/object.h"
#include "seen/mod/types.h"

namespace seen::mod {

class Drawable final : public Object {
 public:
  using Surface = std::shared_ptr<WGPUSurfaceImpl>;
  using WeakSurface = std::weak_ptr<WGPUSurfaceImpl>;
  using Ptr = std::shared_ptr<Drawable>;

  static Ptr Create(const void* drawable, const Surface& surface);
  explicit Drawable(const void* drawable, const Surface& surface);

  // TODO(Autokaka): Implement this method.
  // void Configure();

  Vec<2> GetSize() const;
  void SetSize(const Vec<2>& size);

 private:
  rx::Value<Vec<2>> size_;
  const void* drawable_;
  WeakSurface surface_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Drawable);
};

}  // namespace seen::mod

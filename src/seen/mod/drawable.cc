// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/mod/drawable.h"
#include "seen/base/logger.h"

namespace seen::mod {

Drawable::Ptr Drawable::Create(const void* drawable, const Surface& surface) {
  return std::make_shared<Drawable>(drawable, surface);
}

Drawable::Drawable(const void* drawable, const Surface& surface)
    : Object(Object::Name::kDrawable), drawable_(drawable), surface_(surface) {
  SEEN_ASSERT(drawable_);
  size_.OnNext([this](const Vec<2>& size) {
    auto surface = surface_.lock();
    if (drawable_ != nullptr && surface) {
      pal::drawable_resize(drawable_, size[0], size[1]);
    }
  });
}

Vec<2> Drawable::GetSize() const {
  return size_.Get();
}

void Drawable::SetSize(const Vec<2>& size) {
  size_ = size;
}

}  // namespace seen::mod

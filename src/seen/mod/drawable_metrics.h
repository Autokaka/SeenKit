// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/24.

#pragma once

#include <fmt/format.h>

#include "seen/mod/object.h"

namespace seen::mod {

struct DrawableMetrics final : public Object {
  double width = 0;
  double height = 0;
  double content_scale = 1;

  explicit DrawableMetrics(double width = 0, double height = 0, double content_scale = 1)
      : Object(Object::Name::kDrawableMetrics), width(width), height(height), content_scale(content_scale) {}

  bool operator==(const DrawableMetrics& other) const {
    return width == other.width && height == other.height && content_scale == other.content_scale;
  }
};

}  // namespace seen::mod

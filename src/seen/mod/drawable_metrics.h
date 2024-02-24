// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/24.

#pragma once

#include <fmt/format.h>
#include <string>

namespace seen::mod {

struct DrawableMetrics final {
  double width = 0;
  double height = 0;
  double content_scale = 1;

  bool operator==(const DrawableMetrics& other) const {
    return width == other.width && height == other.height && content_scale == other.content_scale;
  }

  [[nodiscard]] std::string ToString() const {
    return fmt::format("DrawableMetrics(width={}, height={}, content_scale={})", width, height, content_scale);
  }
};

}  // namespace seen::mod

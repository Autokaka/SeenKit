// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/11.

#include "seen/base/rect.h"
#include "seen/base/math.h"

namespace seen {

static_assert(std::numeric_limits<float>::is_iec559);
constexpr auto kInf = std::numeric_limits<float>::infinity();

Rect Rect::Zero() {
  static const Rect zero_rect = Rect(glm::vec2(0.0F), glm::vec2(0.0F));
  return zero_rect;
}

Rect Rect::Null() {
  static const Rect null_rect = Rect(glm::vec2(kInf), glm::vec2(-kInf));
  return null_rect;
}

Rect Rect::Infinite() {
  static const Rect infinite_rect = Rect(glm::vec2(-kInf), glm::vec2(kInf));
  return infinite_rect;
}

Rect Rect::CreateFromMinAndMax(const glm::vec2& min, const glm::vec2& max) {
  return Rect(min, max).Standardize();
}

Rect Rect::CreateFromOriginAndSize(const glm::vec2& origin, const glm::vec2& size) {
  auto max_point = origin + size;
  return Rect(glm::min(origin, max_point), glm::max(origin, max_point)).Standardize();
}

Rect::Rect() : min_(0.0F, 0.0F), max_(0.0F, 0.0F) {}

Rect::Rect(const glm::vec2& min, const glm::vec2& max) : min_(min), max_(max) {}

bool Rect::operator==(const Rect& other) const {
  const auto this_rect = Standardize();
  const auto other_rect = other.Standardize();
  return this_rect.min_ == other_rect.min_ && this_rect.max_ == other_rect.max_;
}

bool Rect::operator!=(const Rect& other) const {
  return !(*this == other);
}

bool Rect::IsEmpty() const {
  if (min_.x >= kInf || min_.y >= kInf) {
    return true;
  }
  if (max_.x <= -kInf || max_.y <= -kInf) {
    return true;
  }
  if (min_.x >= max_.x || min_.y >= max_.y) {
    return true;
  }
  return false;
}

bool Rect::IsNull() const {
  if (min_.x >= kInf || min_.y >= kInf) {
    return true;
  }
  if (max_.x <= -kInf || max_.y <= -kInf) {
    return true;
  }
  if (min_.x > max_.x || min_.y > max_.y) {
    return true;
  }
  return false;
}

bool Rect::IsInfinite() const {
  return (min_.x <= -kInf && min_.y <= -kInf) && (max_.x >= kInf && max_.y >= kInf);
}

glm::vec2 Rect::GetOrigin() const {
  return min_;
}

void Rect::SetOrigin(const glm::vec2& origin) {
  const auto size = GetSize();
  min_ = origin;
  max_ = origin + size;
}

glm::vec2 Rect::GetSize() const {
  return IsNull() ? glm::vec2(0.0F, 0.0F) : (max_ - min_);
}

void Rect::SetSize(const glm::vec2& size) {
  max_ = min_ + size;
}

float Rect::GetMinX() const {
  return min_.x;
}

void Rect::SetMinX(float min_x) {
  min_.x = min_x;
}

float Rect::GetMinY() const {
  return min_.y;
}

void Rect::SetMinY(float min_y) {
  min_.y = min_y;
}

float Rect::GetMaxX() const {
  return max_.x;
}

void Rect::SetMaxX(float max_x) {
  max_.x = max_x;
}

float Rect::GetMaxY() const {
  return max_.y;
}

void Rect::SetMaxY(float max_y) {
  max_.y = max_y;
}

float Rect::GetOriginX() const {
  return min_.x;
}

void Rect::SetOriginX(float origin_x) {
  const auto size = GetSize();
  min_.x = origin_x;
  max_.x = origin_x + size.x;
}

float Rect::GetOriginY() const {
  return min_.y;
}

void Rect::SetOriginY(float origin_y) {
  const auto size = GetSize();
  min_.y = origin_y;
  max_.y = origin_y + size.y;
}

float Rect::GetWidth() const {
  return IsNull() ? 0.0F : (max_.x - min_.x);
}

void Rect::SetWidth(float width) {
  max_.x = min_.x + width;
}

float Rect::GetHeight() const {
  return IsNull() ? 0.0F : (max_.y - min_.y);
}

void Rect::SetHeight(float height) {
  max_.y = min_.y + height;
}

bool Rect::IsEqual(const Rect& other) const {
  return *this == other;
}

bool Rect::IntersectsRect(const Rect& other) const {
  return !Intersection(other).IsEmpty();
}

bool Rect::ContainsPoint(const glm::vec2& point) const {
  return (point.x >= min_.x && point.x < max_.x) && (point.y >= min_.y && point.y < max_.y);
}

bool Rect::ContainsRect(const Rect& other) const {
  return (other.min_.x >= min_.x && other.max_.x <= max_.x) && (other.min_.y >= min_.y && other.max_.y <= max_.y);
}

Rect Rect::Intersection(const Rect& other) const {
  const auto min = glm::max(min_, other.min_);
  const auto max = glm::min(max_, other.max_);
  return Rect(min, max).Standardize();
}

Rect Rect::Union(const Rect& other) const {
  const auto min = glm::min(min_, other.min_);
  const auto max = glm::max(max_, other.max_);
  return Rect(min, max).Standardize();
}

Rect Rect::Transform(const glm::mat3& transform) const {
  if (IsNull() || IsInfinite()) {
    return *this;
  }

  glm::vec2 min;
  glm::vec2 max;
  min = max = math::Apply(transform, min_);

  const auto& transformer = [&](float point_x, float point_y) {
    const auto transformed_point = math::Apply(transform, glm::vec2(point_x, point_y));
    min = glm::min(min, transformed_point);
    max = glm::max(max, transformed_point);
  };

  transformer(min_.x, max_.y);
  transformer(max_.x, min_.y);
  transformer(max_.x, max_.y);

  return Rect(min, max).Standardize();
}

Rect Rect::Standardize() const {
  return IsNull() ? Null() : *this;
}

}  // namespace seen

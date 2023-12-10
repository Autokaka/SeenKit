// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/11.

#pragma once

#include <glm/glm.hpp>

namespace seen {

class Rect final {
 public:
  static Rect Zero();
  static Rect Null();
  static Rect Infinite();

  static Rect CreateFromMinAndMax(const glm::vec2& min, const glm::vec2& max);

  static Rect CreateFromOriginAndSize(const glm::vec2& origin, const glm::vec2& size);

  Rect();
  Rect(const glm::vec2& min, const glm::vec2& max);

  bool operator==(const Rect& other) const;
  bool operator!=(const Rect& other) const;

  [[nodiscard]] bool IsEmpty() const;
  [[nodiscard]] bool IsNull() const;
  [[nodiscard]] bool IsInfinite() const;

  [[nodiscard]] glm::vec2 GetMin() const { return min_; }
  void SetMin(const glm::vec2& min) { min_ = min; }

  [[nodiscard]] glm::vec2 GetMax() const { return max_; }
  void SetMax(const glm::vec2& max) { max_ = max; }

  [[nodiscard]] glm::vec2 GetOrigin() const;
  void SetOrigin(const glm::vec2& origin);

  [[nodiscard]] glm::vec2 GetSize() const;
  void SetSize(const glm::vec2& size);

  [[nodiscard]] float GetMinX() const;
  void SetMinX(float min_x);

  [[nodiscard]] float GetMinY() const;
  void SetMinY(float min_y);

  [[nodiscard]] float GetMaxX() const;
  void SetMaxX(float max_x);

  [[nodiscard]] float GetMaxY() const;
  void SetMaxY(float max_y);

  [[nodiscard]] float GetOriginX() const;
  void SetOriginX(float origin_x);

  [[nodiscard]] float GetOriginY() const;
  void SetOriginY(float origin_y);

  [[nodiscard]] float GetWidth() const;
  void SetWidth(float width);

  [[nodiscard]] float GetHeight() const;
  void SetHeight(float height);

  [[nodiscard]] bool IsEqual(const Rect& other) const;
  [[nodiscard]] bool IntersectsRect(const Rect& other) const;

  [[nodiscard]] bool ContainsPoint(const glm::vec2& point) const;
  [[nodiscard]] bool ContainsRect(const Rect& other) const;

  [[nodiscard]] Rect Intersection(const Rect& other) const;
  [[nodiscard]] Rect Union(const Rect& other) const;
  [[nodiscard]] Rect Transform(const glm::mat3& transform) const;

 private:
  [[nodiscard]] Rect Standardize() const;

  glm::vec2 min_;
  glm::vec2 max_;
};

}  // namespace seen

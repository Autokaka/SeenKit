// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/11.

#pragma once

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "seen/base/rect.h"

namespace seen::math {

inline bool IsAffineTransform(const glm::mat3& matrix) {
  return matrix[0][2] == 0 && matrix[1][2] == 0 && matrix[2][2] == 1;
}

inline bool IsAffineTransform(const glm::mat4& matrix) {
  return matrix[0][3] == 0 && matrix[1][3] == 0 && matrix[2][3] == 1 && matrix[3][3] == 1;
}

inline glm::vec2 Apply(const glm::mat3& matrix, const glm::vec2& point) {
  auto result_3d = matrix * glm::vec3(point, 1.0F);
  result_3d /= result_3d.z;
  return result_3d;
}

inline glm::vec3 Apply(const glm::mat4& matrix, const glm::vec3& point) {
  auto result_4d = matrix * glm::vec4(point, 1.0F);
  result_4d /= result_4d.w;
  return result_4d;
}

inline glm::mat3 Camera(float left, float right, float bottom, float top) {
  const float range = 2.0F;
  glm::mat3 result(1.0F);
  result[0][0] = range / (right - left);
  result[1][1] = range / (top - bottom);
  result[2][0] = -(right + left) / (right - left);
  result[2][1] = -(top + bottom) / (top - bottom);
  return result;
}

inline glm::mat3 Camera(Rect rect, bool flipped = false) {
  auto left = rect.GetMinX();
  auto right = rect.GetMaxX();
  auto bottom = rect.GetMinY();
  auto top = rect.GetMaxY();
  if (flipped) {
    std::swap(bottom, top);
  }
  return math::Camera(left, right, bottom, top);
}

inline glm::vec3 SafeSlerp(const glm::vec3& begin_v, const glm::vec3& end_v, float time) {
  return begin_v == end_v ? begin_v : glm::slerp(begin_v, end_v, time);
}

}  // namespace seen::math

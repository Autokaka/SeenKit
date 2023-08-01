// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/29.

#pragma once

#include <chrono>
#include <cstdint>
#include <iosfwd>

#include "seen/foundation/time_delta.h"

namespace seen {

class TimePoint {
 public:
  constexpr TimePoint() = default;

  static TimePoint Now() {
    const auto elapsed_time = std::chrono::steady_clock::now().time_since_epoch();
    return FromEpochTime(std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed_time));
  }

  static constexpr TimePoint Min() { return TimePoint(std::numeric_limits<int64_t>::lowest()); }

  static constexpr TimePoint Max() { return TimePoint(std::numeric_limits<int64_t>::max()); }

  static constexpr TimePoint FromEpochDelta(TimeDelta ticks) { return TimePoint(ticks.ToNanoseconds()); }

  [[nodiscard]] TimeDelta ToEpochDelta() const { return TimeDelta::FromNanoseconds(ticks_); }

  static constexpr TimePoint FromEpochTime(const std::chrono::nanoseconds& ticks) { return TimePoint(ticks.count()); }

  [[nodiscard]] std::chrono::steady_clock::time_point ToEpochTime() const {
    std::chrono::nanoseconds duration(ticks_);
    return std::chrono::steady_clock::time_point(duration);
  }

  TimeDelta operator-(TimePoint other) const { return TimeDelta::FromNanoseconds(ticks_ - other.ticks_); }

  TimePoint operator+(TimeDelta duration) const { return TimePoint(ticks_ + duration.ToNanoseconds()); }
  TimePoint operator-(TimeDelta duration) const { return TimePoint(ticks_ - duration.ToNanoseconds()); }

  bool operator==(TimePoint other) const { return ticks_ == other.ticks_; }
  bool operator!=(TimePoint other) const { return ticks_ != other.ticks_; }
  bool operator<(TimePoint other) const { return ticks_ < other.ticks_; }
  bool operator<=(TimePoint other) const { return ticks_ <= other.ticks_; }
  bool operator>(TimePoint other) const { return ticks_ > other.ticks_; }
  bool operator>=(TimePoint other) const { return ticks_ >= other.ticks_; }

 private:
  explicit constexpr TimePoint(int64_t ticks) : ticks_(ticks) {}

  int64_t ticks_ = 0;
};

}  // namespace seen

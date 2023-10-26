// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/29.

#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <iosfwd>
#include <limits>

namespace seen {

class TimeDelta {
 public:
  constexpr TimeDelta() = default;

  static constexpr TimeDelta Zero() { return {}; }
  static constexpr TimeDelta Min() { return TimeDelta(std::numeric_limits<int64_t>::lowest()); }
  static constexpr TimeDelta Max() { return TimeDelta(std::numeric_limits<int64_t>::max()); }
  static constexpr TimeDelta FromNanoseconds(int64_t nanos) { return TimeDelta(nanos); }
  static constexpr TimeDelta FromMicroseconds(int64_t micros) { return FromNanoseconds(micros * kTimeScale); }
  static constexpr TimeDelta FromMilliseconds(int64_t millis) { return FromMicroseconds(millis * kTimeScale); }
  static constexpr TimeDelta FromSeconds(int64_t seconds) { return FromMilliseconds(seconds * kTimeScale); }

  static constexpr TimeDelta FromSecondsF(double seconds) {
    return FromNanoseconds(seconds * kTimeScale * kTimeScale * kTimeScale);
  }

  [[nodiscard]] constexpr int64_t ToNanoseconds() const { return delta_; }
  [[nodiscard]] constexpr int64_t ToMicroseconds() const { return ToNanoseconds() / kTimeScale; }
  [[nodiscard]] constexpr int64_t ToMilliseconds() const { return ToMicroseconds() / kTimeScale; }
  [[nodiscard]] constexpr int64_t ToSeconds() const { return ToMilliseconds() / kTimeScale; }

  [[nodiscard]] constexpr double ToNanosecondsF() const { return delta_; }
  [[nodiscard]] constexpr double ToMicrosecondsF() const { return delta_ / kTimeScale; }
  [[nodiscard]] constexpr double ToMillisecondsF() const { return delta_ / (kTimeScale * kTimeScale); }
  [[nodiscard]] constexpr double ToSecondsF() const { return delta_ / (kTimeScale * kTimeScale * kTimeScale); }

  constexpr TimeDelta operator-() const { return TimeDelta::FromNanoseconds(-delta_); }

  constexpr TimeDelta operator-(TimeDelta other) const { return TimeDelta::FromNanoseconds(delta_ - other.delta_); }

  constexpr TimeDelta operator+(TimeDelta other) const { return TimeDelta::FromNanoseconds(delta_ + other.delta_); }

  constexpr TimeDelta operator/(int64_t divisor) const { return TimeDelta::FromNanoseconds(delta_ / divisor); }

  constexpr TimeDelta operator/(double divisor) const { return TimeDelta::FromNanoseconds(delta_ / divisor); }

  constexpr double operator/(TimeDelta other) const { return delta_ / other.delta_; }

  constexpr TimeDelta operator*(int64_t multiplier) const { return TimeDelta::FromNanoseconds(delta_ * multiplier); }

  constexpr TimeDelta operator*(double multiplier) const { return TimeDelta::FromNanoseconds(delta_ * multiplier); }

  constexpr TimeDelta operator%(TimeDelta other) const { return TimeDelta::FromNanoseconds(delta_ % other.delta_); }

  bool operator==(TimeDelta other) const { return delta_ == other.delta_; }
  bool operator!=(TimeDelta other) const { return delta_ != other.delta_; }
  bool operator<(TimeDelta other) const { return delta_ < other.delta_; }
  bool operator<=(TimeDelta other) const { return delta_ <= other.delta_; }
  bool operator>(TimeDelta other) const { return delta_ > other.delta_; }
  bool operator>=(TimeDelta other) const { return delta_ >= other.delta_; }

  static constexpr TimeDelta FromTimespec(struct timespec timespec_v) {
    return TimeDelta::FromSeconds(timespec_v.tv_sec) + TimeDelta::FromNanoseconds(timespec_v.tv_nsec);
  }

  [[nodiscard]] struct timespec ToTimespec() const {
    struct timespec timespec_v;
    constexpr int64_t kNanosecondsPerSecond = 1000000000LL;
    timespec_v.tv_sec = static_cast<time_t>(ToSeconds());
    timespec_v.tv_nsec = delta_ % kNanosecondsPerSecond;
    return timespec_v;
  }

 private:
  explicit constexpr TimeDelta(int64_t delta) : delta_(delta) {}

  static constexpr int kTimeScale = 1000;
  int64_t delta_ = 0;
};

}  // namespace seen

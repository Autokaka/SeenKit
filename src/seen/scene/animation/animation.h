// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/02.

#pragma once

#include <glm/glm.hpp>

#include "seen/base/time_delta.h"

namespace seen {

class Engine;

namespace scene {

class ValueAnimation;
class TweenAnimation;
class MultiChildAnimation;

class Animation {
 public:
  using Ptr = std::unique_ptr<Animation>;
  using Token = const void*;
  enum class Type { kValue, kTween, kMultiChild };
  enum class State { kIdle, kRunning, kFinished, kCancelled };

  static ValueAnimation Value(double from_value, double to_value, const TimeDelta& duration);
  static ValueAnimation ValueBy(double from_value, double value_delta, const TimeDelta& duration);

  static TweenAnimation Tween(const glm::vec2& from_value, const glm::vec2& to_value, const TimeDelta& duration);
  static TweenAnimation TweenBy(const glm::vec2& from_value, const glm::vec2& value_delta, const TimeDelta& duration);

  static MultiChildAnimation MultiChild(const std::vector<Ptr>& animations, bool parallel = false);
  static MultiChildAnimation Parallel(const std::vector<Ptr>& animations);
  static MultiChildAnimation Sequence(const std::vector<Ptr>& animations);

  static void Cancel(Token token);

  explicit Animation(const Type& type);
  virtual ~Animation();

  // TODO(Autokaka): TimingFunction

  [[nodiscard]] Type GetType() const;
  [[nodiscard]] virtual TimeDelta GetDuration() const = 0;

 protected:
  static Token Register(const Ptr& animation);
  virtual State Update(const TimeDelta& time_delta, bool cancel) = 0;

 private:
  friend class seen::Engine;
  friend class MultiChildAnimation;

  static void UpdateAll(const TimeDelta& time_delta);

  Type type_;
};

}  // namespace scene
}  // namespace seen

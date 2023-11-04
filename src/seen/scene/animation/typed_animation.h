// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/03.

#pragma once

#include <functional>
#include <glm/glm.hpp>

#include "seen/base/time_delta.h"
#include "seen/base/time_point.h"
#include "seen/scene/animation/animation.h"

namespace seen::scene {

template <typename T, Animation::Type AnimationType>
class TypedAnimation : public Animation {
 public:
  using OnUpdateCallback = std::function<void(const TweenAnimation*, const State&, const T&)>;

  explicit TypedAnimation(const T& from_value, const T& to_value, const TimeDelta& duration)
      : Animation(AnimationType), from_(from_value), to_(to_value), duration_(std::max(TimeDelta::Zero(), duration)) {}
  ~TypedAnimation() override = default;

  Token Start(OnUpdateCallback on_update) {
    auto animation_copy = std::make_unique<TypedAnimation>(*this);
    auto now = TimePoint::Now();
    animation_copy->begin_time_ = now;
    animation_copy->current_time_ = now;
    animation_copy->on_update_callback_ = std::move(on_update);
    return Register(std::move(animation_copy));
  }

  [[nodiscard]] TimeDelta GetDuration() const override { return duration_; }

 private:
  friend class Animation;

  State Update(const TimeDelta& time_delta, bool cancel) override {
    if (state_ == State::kFinished || state_ == State::kCancelled) {
      return state_;
    }
    auto current = from_;
    state_ = UpdateInternal(time_delta, &current);
    state_ = cancel ? State::kCancelled : state_;
    if (on_update_callback_) {
      on_update_callback_(this, state_, current);
    }
    return state_;
  }

  State UpdateInternal(const TimeDelta& time_delta, T* current) {
    current_time_ = current_time_ + time_delta;
    if (current_time_ < begin_time_) {
      *current = to_;
      return State::kIdle;
    }
    if (current_time_ >= begin_time_ + duration_) {
      *current = to_;
      return State::kFinished;
    }
    *current = from_ + (to_ - from_) * (current_time_ - begin_time_) / duration_;
    return State::kRunning;
  }

  State state_;
  T from_;
  T to_;

  TimePoint begin_time_;
  TimeDelta duration_;

  TimePoint current_time_;
  OnUpdateCallback on_update_callback_;
};

class ValueAnimation final : public TypedAnimation<double, Animation::Type::kValue> {
 public:
  using TypedAnimation::TypedAnimation;
};

class TweenAnimation final : public TypedAnimation<glm::vec2, Animation::Type::kTween> {
 public:
  using TypedAnimation::TypedAnimation;
};

}  // namespace seen::scene

// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/03.

#include <list>

#include "seen/scene/animation/animation.h"
#include "seen/scene/animation/multi_child_animation.h"
#include "seen/scene/animation/typed_animation.h"

namespace seen::scene {

thread_local std::list<Animation::Ptr> tls_animations_;

ValueAnimation Animation::Value(double from_value, double to_value, const TimeDelta& duration) {
  return ValueAnimation{from_value, to_value, duration};
}

ValueAnimation Animation::ValueBy(double from_value, double value_delta, const TimeDelta& duration) {
  return Value(from_value, from_value + value_delta, duration);
}

TweenAnimation Animation::Tween(const glm::vec2& from_value, const glm::vec2& to_value, const TimeDelta& duration) {
  return TweenAnimation{from_value, to_value, duration};
}

TweenAnimation Animation::TweenBy(const glm::vec2& from_value,
                                  const glm::vec2& value_delta,
                                  const TimeDelta& duration) {
  return Tween(from_value, from_value + value_delta, duration);
}

MultiChildAnimation Animation::MultiChild(const std::vector<Ptr>& animations, bool parallel) {
  return MultiChildAnimation(animations, parallel);
}

MultiChildAnimation Animation::Parallel(const std::vector<Ptr>& animations) {
  return MultiChild(animations, true);
}

MultiChildAnimation Animation::Sequence(const std::vector<Ptr>& animations) {
  return MultiChild(animations, false);
}

void Animation::Cancel(Animation::Token token) {
  tls_animations_.remove_if([token](const Ptr& animation) {
    if (reinterpret_cast<Token>(&animation) == token) {
      animation->Update(TimeDelta::Zero(), true);
      return true;
    }
    return false;
  });
}

Animation::Animation(const Type& type) : type_(type) {}

Animation::~Animation() = default;

Animation::Type Animation::GetType() const {
  return type_;
}

Animation::Token Animation::Register(const Ptr& animation) {
  tls_animations_.emplace_back(animation);
  return &tls_animations_.back();
}

void Animation::UpdateAll(const TimeDelta& time_delta) {
  for (const auto& animation : tls_animations_) {
    auto state = animation->Update(time_delta, false);
    if (state == Animation::State::kFinished) {
      tls_animations_.remove(animation);
    }
  }
}

}  // namespace seen::scene

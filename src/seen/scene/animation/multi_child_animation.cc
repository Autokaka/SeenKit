// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/03.

#include "seen/scene/animation/multi_child_animation.h"
#include "seen/scene/animation/typed_animation.h"

namespace seen::scene {

MultiChildAnimation::MultiChildAnimation(const std::vector<Ptr>& animations, bool parallel)
    : Animation(Type::kMultiChild), parallel_(parallel) {
  CopyAnimations(animations);
}

MultiChildAnimation::~MultiChildAnimation() = default;

MultiChildAnimation::MultiChildAnimation(const MultiChildAnimation& other)
    : MultiChildAnimation(other.children_, other.parallel_) {
  state_ = other.state_;
}

MultiChildAnimation::MultiChildAnimation(MultiChildAnimation&& other) noexcept
    : Animation(other.GetType()),
      state_(other.state_),
      children_(std::move(other.children_)),
      parallel_(other.parallel_) {}

MultiChildAnimation& MultiChildAnimation::operator=(const MultiChildAnimation& other) {
  if (this == &other) {
    return *this;
  }
  state_ = other.state_;
  CopyAnimations(other.children_);
  parallel_ = other.parallel_;
  return *this;
}

MultiChildAnimation& MultiChildAnimation::operator=(MultiChildAnimation&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  state_ = other.state_;
  children_ = std::move(other.children_);
  parallel_ = other.parallel_;
  return *this;
}

Animation::Token MultiChildAnimation::Start(OnUpdateCallback on_update) {
  auto animation_copy = std::make_unique<MultiChildAnimation>(*this);
  animation_copy->on_update_callback_ = std::move(on_update);
  return Register(std::move(animation_copy));
}

[[nodiscard]] TimeDelta MultiChildAnimation::GetDuration() const {
  auto duration = TimeDelta::Zero();

  if (parallel_) {
    for (const auto& animation : children_) {
      duration = std::max(duration, animation->GetDuration());
    }
    return duration;
  }

  for (const auto& animation : children_) {
    duration = duration + animation->GetDuration();
  }
  return duration;
}

[[nodiscard]] bool MultiChildAnimation::IsParallel() const {
  return parallel_;
}

Animation::State MultiChildAnimation::Update(const TimeDelta& time_delta, bool cancel) {
  if (state_ == State::kFinished || state_ == State::kCancelled) {
    return state_;
  }
  state_ = UpdateInternal(time_delta);
  state_ = cancel ? State::kCancelled : state_;
  if (on_update_callback_) {
    on_update_callback_(this, state_);
  }
  return state_;
}

Animation::State MultiChildAnimation::UpdateInternal(const TimeDelta& time_delta) {
  if (parallel_) {
    int finish_count = 0;
    for (const auto& animation : children_) {
      auto result = animation->Update(time_delta, false);
      if (result == State::kFinished) {
        finish_count += 1;
      }
    }
    auto all_finished = finish_count == children_.size();
    return all_finished ? State::kFinished : State::kRunning;
  }

  for (const auto& animation : children_) {
    auto state = animation->Update(time_delta, false);
    if (state != State::kFinished) {
      return State::kRunning;
    }
  }
  return State::kFinished;
}

void MultiChildAnimation::CopyAnimations(const std::vector<Ptr>& animations) {
  children_.clear();
  for (const auto& animation : animations) {
    switch (animation->GetType()) {
      case Type::kValue: {
        auto animation_copy = *static_cast<ValueAnimation*>(animation.get());
        children_.emplace_back(std::make_unique<ValueAnimation>(animation_copy));
        break;
      }
      case Type::kTween: {
        auto animation_copy = *static_cast<TweenAnimation*>(animation.get());
        children_.emplace_back(std::make_unique<TweenAnimation>(animation_copy));
        break;
      }
      case Type::kMultiChild: {
        auto animation_copy = *static_cast<MultiChildAnimation*>(animation.get());
        children_.emplace_back(std::make_unique<MultiChildAnimation>(animation_copy));
        break;
      }
    }
  }
}

}  // namespace seen::scene

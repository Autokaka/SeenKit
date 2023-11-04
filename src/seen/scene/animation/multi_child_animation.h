// Created by Autokaka (qq1909698494@gmail.com) on 2023/11/03.

#pragma once

#include <functional>
#include <vector>

#include "seen/scene/animation/animation.h"

namespace seen::scene {

class MultiChildAnimation final : public Animation {
 public:
  using OnUpdateCallback = std::function<void(const MultiChildAnimation*, const State&)>;

  explicit MultiChildAnimation(const std::vector<Ptr>& animations, bool parallel = false);
  ~MultiChildAnimation() override;

  MultiChildAnimation(const MultiChildAnimation& other);
  MultiChildAnimation(MultiChildAnimation&& other) noexcept;

  MultiChildAnimation& operator=(const MultiChildAnimation& other);
  MultiChildAnimation& operator=(MultiChildAnimation&& other) noexcept;

  Token Start(OnUpdateCallback on_update);
  [[nodiscard]] TimeDelta GetDuration() const override;
  [[nodiscard]] bool IsParallel() const;

 private:
  friend class Animation;

  State Update(const TimeDelta& time_delta, bool cancel) override;
  State UpdateInternal(const TimeDelta& time_delta);
  void CopyAnimations(const std::vector<Ptr>& animations);

  State state_;
  std::vector<Ptr> children_;
  bool parallel_;
  OnUpdateCallback on_update_callback_;
};

}  // namespace seen::scene

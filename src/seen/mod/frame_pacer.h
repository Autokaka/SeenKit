// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/25.

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <optional>

#include "seen/base/time_point.h"
#include "seen/base/worker.h"
#include "seen/mod/object.h"

namespace seen::mod {

class FramePacer final : public Object {
 public:
  friend class Seen;
  using Ptr = std::shared_ptr<FramePacer>;
  using WeakPtr = std::weak_ptr<FramePacer>;
  using FrameCallback = std::function<void(std::int64_t now, std::int64_t output)>;

  std::size_t RequestAnimationFrame(FrameCallback callback);
  void CancelAnimationFrame(std::size_t token);

  static Ptr Create(const Worker::Ptr& runner);
  explicit FramePacer(const Worker::Ptr& runner);

 private:
  struct FrameTask {
    std::size_t token;
    FrameCallback callback;
    bool cancelled;
  };

  void AwaitVsync();
  void OnVsync();
  void OnVsync(const TimePoint& frame_display_time);
  void OnRunnerVsync(const TimePoint& frame_display_time);

  Worker::WeakPtr runner_;
  std::shared_ptr<void> handle_;
  std::list<FrameTask> tasks_;
  std::optional<TimePoint> first_frame_time_;
  bool is_pending_;
};

}  // namespace seen::mod

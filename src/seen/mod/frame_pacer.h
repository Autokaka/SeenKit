// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/25.

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <list>
#include <memory>
#include <optional>

#include "seen/base/time_delta.h"
#include "seen/base/time_point.h"
#include "seen/base/worker.h"
#include "seen/mod/object.h"

namespace seen::mod {

class FramePacer final : public Object {
 public:
  struct FrameTime {
    std::int64_t last;
    std::int64_t now;
    std::int64_t output;
  };
  friend class Seen;
  using Ptr = std::shared_ptr<FramePacer>;
  using WeakPtr = std::weak_ptr<FramePacer>;
  using FrameCallback = std::function<void(const FrameTime& time)>;

  std::size_t RequestAnimationFrame(FrameCallback callback);
  void CancelAnimationFrame(std::size_t token);

  static Ptr Create(const CFWorker::Ptr& runner);
  explicit FramePacer(const CFWorker::Ptr& runner);

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

  CFWorker::WeakPtr runner_;
  std::shared_ptr<void> handle_;
  std::list<FrameTask> tasks_;
  std::optional<TimePoint> first_frame_time_;
  TimeDelta prev_frame_end_time_;
  bool is_pending_;
};

}  // namespace seen::mod

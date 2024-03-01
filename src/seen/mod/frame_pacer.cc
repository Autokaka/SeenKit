// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/25.

#include "seen/mod/frame_pacer.h"
#include "seen/base/logger.h"
#include "seen/base/time_point.h"
#include "seen/pal/pal.h"

namespace seen::mod {

FramePacer::Ptr FramePacer::Create(const Worker::Ptr& runner) {
  return std::make_shared<FramePacer>(runner);
}

FramePacer::FramePacer(const Worker::Ptr& runner)
    : Object(Object::Name::kFramePacer),
      runner_(runner),
      handle_(pal::vsync_waiter_create(), pal::vsync_waiter_release),
      first_frame_time_(std::nullopt),
      is_pending_(false) {}

std::size_t FramePacer::RequestAnimationFrame(FrameCallback callback) {
  if (!first_frame_time_) {
    first_frame_time_ = TimePoint::Now();
  }
  auto token = tasks_.size();
  tasks_.emplace_back<FrameTask>({
      .token = token,
      .callback = std::move(callback),
      .cancelled = false,
  });
  AwaitVsync();
  return token;
}

void FramePacer::CancelAnimationFrame(std::size_t token) {
  if (token < 0 || token >= tasks_.size()) {
    return;
  }
  for (auto&& task : tasks_) {
    if (task.token == token) {
      task.cancelled = true;
      return;
    }
  }
}

void FramePacer::AwaitVsync() {
  if (is_pending_) {
    return;
  }
  is_pending_ = true;
  auto weak_self = WeakSelf<FramePacer>();
  pal::vsync_waiter_await(handle_.get(), [weak_self](std::optional<TimePoint> frame_display_time) {
    if (auto self = weak_self.lock()) {
      if (frame_display_time.has_value()) {
        self->OnVsync(frame_display_time.value());
      } else {
        self->OnVsync();
      }
    }
  });
}

// TODO(Autokaka): Calculate approximate `current_frame_due_millis` if none is provided by `pal`.
// Could refer to https://github.com/blurbusters/RefreshRateCalculator
void FramePacer::OnVsync() {
  // NOLINTNEXTLINE(readability-magic-numbers)
  // auto target_time = TimePoint::Now().ToEpochDelta().ToMillisecondsF() + 16.66;
  // OnVsync(static_cast<std::size_t>(target_time));
}

void FramePacer::OnVsync(const TimePoint& frame_display_time) {
  if (auto runner = runner_.lock()) {
    if (runner->IsCurrent()) {
      OnRunnerVsync(frame_display_time);
      return;
    }
    auto weak_self = WeakSelf<FramePacer>();
    runner->DispatchAsync([weak_self, frame_display_time]() {
      if (auto self = weak_self.lock()) {
        self->OnRunnerVsync(frame_display_time);
      }
    });
  }
}

void FramePacer::OnRunnerVsync(const TimePoint& frame_display_time) {
  is_pending_ = false;
  auto now = TimePoint::Now();
  if (now >= frame_display_time) {
    SEEN_DEBUG("Vsync callback expired, try next vsync...");
    AwaitVsync();
    return;
  }
  auto now_time = now - first_frame_time_.value();
  auto display_time = frame_display_time - first_frame_time_.value();
  std::list<FrameTask> tasks;
  std::list<FrameTask> timeout_tasks;
  std::swap(tasks, tasks_);
  for (auto&& task : tasks) {
    if (task.cancelled) {
      continue;
    }
    if (TimePoint::Now() < frame_display_time) {
      task.callback(now_time.ToMilliseconds(), display_time.ToMilliseconds());
    } else {
      timeout_tasks.emplace_back(std::move(task));
    }
  }
  auto timeout_count = timeout_tasks.size();
  if (timeout_count > 0) {
    SEEN_DEBUG("{} frame(s) timeout, try to callback on next vsync...", timeout_count);
    tasks_.splice(tasks_.end(), std::move(timeout_tasks));
    AwaitVsync();
  }
}

}  // namespace seen::mod

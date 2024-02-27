// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/25.

#include "seen/mod/frame_pacer.h"
#include "seen/base/logger.h"
#include "seen/base/time_point.h"
#include "seen/pal/pal.h"

namespace seen::mod {

FramePacer::Ptr FramePacer::Create(const CFWorker::Ptr& runner) {
  return std::make_shared<FramePacer>(runner);
}

FramePacer::FramePacer(const CFWorker::Ptr& runner)
    : Object(Object::Name::kFramePacer),
      runner_(runner),
      handle_(pal::vsync_waiter_create(), pal::vsync_waiter_release),
      first_frame_time_(std::nullopt),
      prev_frame_end_time_(TimeDelta::Zero()),
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
  if (TimePoint::Now() >= frame_display_time) {
    SEEN_DEBUG("Vsync callback late, skip to next vsync callback...");
    AwaitVsync();
    return;
  }
  auto prev_frame_end_millis = prev_frame_end_time_.ToMilliseconds();
  auto display_time = frame_display_time - first_frame_time_.value();
  std::list<FrameTask> tasks;
  std::list<FrameTask> timeout_tasks;
  std::swap(tasks, tasks_);
  is_pending_ = false;
  for (auto&& task : tasks) {
    if (task.cancelled) {
      continue;
    }
    auto now = TimePoint::Now();
    if (now >= frame_display_time) {
      timeout_tasks.emplace_back(std::move(task));
      continue;
    }
    auto now_millis = now - first_frame_time_.value();
    task.callback({
        .last = prev_frame_end_millis,
        .now = now_millis.ToMilliseconds(),
        .output = display_time.ToMilliseconds(),
    });
  }
  auto timeout_count = timeout_tasks.size();
  if (timeout_count > 0) {
    SEEN_DEBUG("{} frame(s) timeout, skip to next vsync callback...", timeout_count);
    tasks_.splice(tasks_.end(), std::move(timeout_tasks));
    AwaitVsync();
  }
  prev_frame_end_time_ = TimePoint::Now() - first_frame_time_.value();
}

}  // namespace seen::mod

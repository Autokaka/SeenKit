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
      tasks_count_(0),
      prev_frame_end_time_(TimeDelta::Zero()),
      is_pending_(false) {}

std::size_t FramePacer::RequestAnimationFrame(FrameCallback callback) {
  auto token = tasks_count_;
  FrameTask task{token, std::move(callback), false};
  tasks_.emplace_back(std::move(task));
  tasks_count_ += 1;
  if (!is_pending_) {
    is_pending_ = true;
    AwaitVsync();
  }
  return token;
}

void FramePacer::CancelAnimationFrame(std::size_t token) {
  if (token >= tasks_count_) {
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
  auto weak_self = WeakSelf<FramePacer>();
  pal::vsync_waiter_await(handle_.get(), [weak_self](std::optional<std::size_t> current_frame_due_millis) {
    if (auto self = weak_self.lock()) {
      if (current_frame_due_millis.has_value()) {
        self->OnVsync(*current_frame_due_millis);
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
  auto target_time = TimePoint::Now().ToEpochDelta().ToMillisecondsF() + 16.66;
  OnVsync(static_cast<std::size_t>(target_time));
}

void FramePacer::OnVsync(std::size_t current_frame_due_millis) {
  if (auto runner = runner_.lock()) {
    if (runner->IsCurrent()) {
      OnRunnerVsync(current_frame_due_millis);
      return;
    }
    auto weak_self = WeakSelf<FramePacer>();
    auto time = current_frame_due_millis;
    runner->DispatchAsync([weak_self, time]() {
      if (auto self = weak_self.lock()) {
        self->OnRunnerVsync(time);
      }
    });
  }
}

void FramePacer::OnRunnerVsync(std::size_t current_frame_due_millis) {
  auto now = TimePoint::Now();
  auto now_millis = now.ToEpochDelta().ToMilliseconds();
  if (now_millis >= current_frame_due_millis) {
    SEEN_DEBUG("Frame has been skipped to next vsync callback...");
    AwaitVsync();
    return;
  }
  auto prev_frame_end_millis = prev_frame_end_time_.ToMilliseconds();
  for (auto&& task : tasks_) {
    if (task.cancelled) {
      continue;
    }
    task.callback(prev_frame_end_millis, current_frame_due_millis);
  }
  is_pending_ = false;
  tasks_.clear();
  tasks_count_ = 0;
  prev_frame_end_time_ = prev_frame_end_time_ + (TimePoint::Now() - now);
}

}  // namespace seen::mod

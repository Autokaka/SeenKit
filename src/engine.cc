// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "engine.h"
#include "seen/base/logger.h"

namespace seen {

const char* const kMainWorkerName = "Seen.Main";

Engine::Engine()
    : main_worker_(CFWorker::Create(kMainWorkerName)),
      main_channel_(std::make_shared<CFDataChannel>(main_worker_, platform_channel_)),
      platform_channel_(std::make_shared<CFDataChannel>(GetPlatformWorker(), main_channel_)) {}

CFDataChannel::Ptr Engine::GetChannel() const {
  return platform_channel_;
}

void Engine::Update(const TimeDelta& time_delta, const CFClosure& on_complete) {
  auto begin_time = TimePoint::Now();
  main_worker_->DispatchAsync([begin_time]() {
    auto end_time = TimePoint::Now();
    SEEN_INFO("Update, time_delta is {}ms", (end_time - begin_time).ToMilliseconds());
  });
}

}  // namespace seen

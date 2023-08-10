// Created by Autokaka (qq1909698494@gmail.com) on 2023/03/03.

#include "engine.h"
#include "seen/foundation/logger.h"

namespace seen {

const char* Engine::kMainWorkerName = "Seen.Main";

Engine::Engine(const CFData::Ptr& bundle_data) : worker_(CreateWorker(kMainWorkerName)) {
  SEEN_ASSERT(bundle_data);
}

CFDataChannel::Ptr Engine::CreateChannel() const {
  return std::make_shared<CFDataChannel>(worker_);
}

void Engine::Update(const TimeDelta& time_delta, const EngineDidUpdateCallback& callback) {
  worker_->DispatchAsync([callback]() {
    // update scene

    GetPlatformWorker()->DispatchAsync([callback]() {
      if (callback) {
        // callback();
      }
    });
  });
}

}  // namespace seen

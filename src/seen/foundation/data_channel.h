// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/10.

#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "seen/foundation/class_constraints.h"
#include "seen/foundation/data.h"
#include "seen/foundation/worker.h"

namespace seen {

class CFDataChannel final : public std::enable_shared_from_this<CFDataChannel> {
 public:
  DISALLOW_COPY_ASSIGN_AND_MOVE(CFDataChannel);
  using Ptr = std::shared_ptr<CFDataChannel>;
  using DataHandler = std::function<void(const CFData::Ptr&)>;

  explicit CFDataChannel(const CFWorker::Ptr& worker);
  void SendData(const CFData::Ptr& data, bool move = false);
  void SetDataHandler(const DataHandler& data_handler);

 private:
  std::mutex mutex_;
  CFWorker::Weak weak_worker_;
  DataHandler data_handler_;
};

}  // namespace seen

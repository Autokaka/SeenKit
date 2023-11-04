// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/10.

#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "seen/base/class_ext.h"
#include "seen/base/data.h"
#include "seen/base/worker.h"

namespace seen {

class CFDataChannel final : public std::enable_shared_from_this<CFDataChannel> {
 public:
  using Ptr = std::shared_ptr<CFDataChannel>;
  using WeakPtr = std::weak_ptr<CFDataChannel>;
  using DataHandler = std::function<void(const CFData::Ptr&)>;

  explicit CFDataChannel(const CFWorker::Ptr& worker, const CFDataChannel::Ptr& paired_channel);
  void SendData(const CFData::Ptr& data, bool move = false);
  void SetDataHandler(const DataHandler& data_handler);
  const DataHandler& GetDataHandler();

 private:
  void ReceiveData(const CFData::Ptr& data);

  std::mutex mutex_;
  CFWorker::WeakPtr weak_worker_;
  DataHandler data_handler_;
  CFDataChannel::WeakPtr paired_channel_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFDataChannel);
};

}  // namespace seen

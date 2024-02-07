// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/10.

#include "seen/base/data_channel.h"

namespace seen {

CFDataChannel::Ptr CFDataChannel::Create(const CFWorker::Ptr& worker, const CFDataChannel::Ptr& paired_channel) {
  return std::make_shared<CFDataChannel>(worker, paired_channel);
}

CFDataChannel::CFDataChannel(const CFWorker::Ptr& worker, const CFDataChannel::Ptr& paired_channel)
    : weak_worker_(worker), paired_channel_(paired_channel) {}

void CFDataChannel::SendData(const CFData::Ptr& data, bool move) {
  if (auto worker = weak_worker_.lock()) {
    auto transfer_data = move ? data->Move() : data->Copy();
    worker->DispatchAsync([weak_channel = paired_channel_, transfer_data]() {
      if (auto strong_channel = weak_channel.lock()) {
        strong_channel->ReceiveData(transfer_data);
      }
    });
  }
}

void CFDataChannel::SetDataHandler(const DataHandler& data_handler) {
  std::scoped_lock lock(mutex_);
  data_handler_ = data_handler;
}

const CFDataChannel::DataHandler& CFDataChannel::GetDataHandler() {
  std::scoped_lock lock(mutex_);
  return data_handler_;
}

void CFDataChannel::ReceiveData(const CFData::Ptr& data) {
  std::scoped_lock lock(mutex_);
  if (data_handler_) {
    data_handler_(data);
  }
}

}  // namespace seen

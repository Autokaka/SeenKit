// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/10.

#include "seen/base/data_channel.h"

namespace seen {

DataChannel::Ptr DataChannel::Create(const Worker::Ptr& worker, const DataChannel::Ptr& paired_channel) {
  return std::make_shared<DataChannel>(worker, paired_channel);
}

DataChannel::DataChannel(const Worker::Ptr& worker, const DataChannel::Ptr& paired_channel)
    : weak_worker_(worker), paired_channel_(paired_channel) {}

void DataChannel::SendData(const Data::Ptr& data, bool move) {
  if (auto worker = weak_worker_.lock()) {
    auto transfer_data = move ? data->Move() : data->Copy();
    worker->DispatchAsync([weak_channel = paired_channel_, transfer_data]() {
      if (auto strong_channel = weak_channel.lock()) {
        strong_channel->ReceiveData(transfer_data);
      }
    });
  }
}

void DataChannel::SetDataHandler(const DataHandler& data_handler) {
  std::scoped_lock lock(mutex_);
  data_handler_ = data_handler;
}

const DataChannel::DataHandler& DataChannel::GetDataHandler() {
  std::scoped_lock lock(mutex_);
  return data_handler_;
}

void DataChannel::ReceiveData(const Data::Ptr& data) {
  std::scoped_lock lock(mutex_);
  if (data_handler_) {
    data_handler_(data);
  }
}

}  // namespace seen

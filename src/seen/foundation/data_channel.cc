// Created by Autokaka (qq1909698494@gmail.com) on 2023/08/10.

#include "seen/foundation/data_channel.h"

namespace seen {

CFDataChannel::CFDataChannel(const CFWorker::Ptr& worker) : weak_worker_(worker) {}

void CFDataChannel::SendData(const CFData::Ptr& data, bool move) {
  if (auto worker = weak_worker_.lock()) {
    auto transfer_data = move ? data->Move() : data->Copy();
    worker->DispatchAsync([weak_self = weak_from_this(), transfer_data]() {
      if (auto self = weak_self.lock()) {
        std::scoped_lock lock(self->mutex_);
        if (auto data_handler = self->data_handler_) {
          data_handler(transfer_data);
        }
      }
    });
  }
}

void CFDataChannel::SetDataHandler(const DataHandler& data_handler) {
  std::scoped_lock lock(mutex_);
  data_handler_ = data_handler;
}

}  // namespace seen

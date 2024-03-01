// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <functional>
#include <memory>
#include <mutex>

#include "seen/base/class_ext.h"
#include "seen/base/data.h"
#include "seen/base/worker.h"

namespace seen {

class DataChannel final : public std::enable_shared_from_this<DataChannel> {
 public:
  using Ptr = std::shared_ptr<DataChannel>;
  using WeakPtr = std::weak_ptr<DataChannel>;
  using DataHandler = std::function<void(const Data::Ptr&)>;

  static Ptr Create(const Worker::Ptr& worker, const DataChannel::Ptr& paired_channel);
  explicit DataChannel(const Worker::Ptr& worker, const DataChannel::Ptr& paired_channel);
  void SendData(const Data::Ptr& data, bool move = false);
  void SetDataHandler(const DataHandler& data_handler);
  const DataHandler& GetDataHandler();

 private:
  void ReceiveData(const Data::Ptr& data);

  std::mutex mutex_;
  Worker::WeakPtr weak_worker_;
  DataHandler data_handler_;
  DataChannel::WeakPtr paired_channel_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(DataChannel);
};

}  // namespace seen

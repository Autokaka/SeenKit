// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "seen/base/deferred_task.h"

namespace seen {

DeferredTask::DeferredTask(Closure callback) : callback_(std::move(callback)) {}

DeferredTask::~DeferredTask() {
  callback_();
}

}  // namespace seen

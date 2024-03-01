// Copyright 2024 Autokaka. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "seen/base/class_ext.h"
#include "seen/base/types.h"

namespace seen {

class DeferredTask {
 public:
  explicit DeferredTask(Closure callback);
  ~DeferredTask();

 private:
  Closure callback_;
  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(DeferredTask);
};

}  // namespace seen

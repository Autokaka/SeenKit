// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

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

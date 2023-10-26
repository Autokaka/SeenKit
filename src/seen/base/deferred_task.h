// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#pragma once

#include "seen/base/class_ext.h"
#include "seen/base/types.h"

namespace seen {

class CFDeferredTask {
 public:
  explicit CFDeferredTask(CFClosure callback);
  ~CFDeferredTask();

 private:
  CFClosure callback_;
  DISALLOW_COPY_ASSIGN_AND_MOVE(CFDeferredTask);
};

}  // namespace seen

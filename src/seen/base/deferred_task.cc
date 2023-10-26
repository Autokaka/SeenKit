// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#include "seen/base/deferred_task.h"

namespace seen {

CFDeferredTask::CFDeferredTask(CFClosure callback) : callback_(std::move(callback)) {}

CFDeferredTask::~CFDeferredTask() {
  callback_();
}

}  // namespace seen

// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#include "seen/base/deferred_task.h"

namespace seen {

DeferredTask::DeferredTask(Closure callback) : callback_(std::move(callback)) {}

DeferredTask::~DeferredTask() {
  callback_();
}

}  // namespace seen

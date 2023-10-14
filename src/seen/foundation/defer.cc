// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#include "seen/foundation/defer.h"

namespace seen {

CFDefer::CFDefer(CFClosure callback) : callback_(std::move(callback)) {}

CFDefer::~CFDefer() {
  callback_();
}

}  // namespace seen

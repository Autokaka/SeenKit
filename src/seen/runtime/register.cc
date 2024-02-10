// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/09.

#include "seen/runtime/register.h"
#include "seen/base/logger.h"
#include "seen/runtime/bind.h"

namespace seen::runtime {

namespace {

void TestLog(const char* message) {
  SEEN_INFO("{}", message);
}

}  // namespace

void RegisterBindings() {
  Bind<TestLog>("log");
}

}  // namespace seen::runtime

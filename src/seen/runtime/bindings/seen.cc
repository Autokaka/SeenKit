// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/bindings/seen.h"
#include "seen/base/logger.h"
#include "seen/runtime/bindings/gpu.h"

namespace seen::runtime {

Local<Value> Seen::Log(const Arguments& args) {
  std::string message;
  for (int i = 0; i < args.size(); ++i) {
    message += args[i].describeUtf8() + " ";
  }
  SEEN_INFO("{}", message);
  return {};
}

Local<Value> Seen::GetVersion() {
  return String::newString(SEEN_VERSION);
}

Local<Value> Seen::GetGPU() {
  static auto* gpu = GPU::Create();
  if (gpu == nullptr) {
    return {};
  }
  return gpu->getScriptObject();
}

}  // namespace seen::runtime

// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <ScriptX/ScriptX.h>

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace script;

class Seen : public ScriptClass {
 public:
  using ScriptClass::ScriptClass;

  static Local<Value> Log(const Arguments& args);

  static Local<Value> GetVersion();

  static Local<Value> GetGPU();
};

}  // namespace seen::runtime

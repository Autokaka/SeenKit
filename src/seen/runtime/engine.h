// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/08.

#include <wasm_export.h>
#include <memory>
#include <vector>

#include "seen/base/class_ext.h"

#pragma once

namespace seen::runtime {

class Engine final {
 public:
  using Ptr = std::unique_ptr<Engine>;

  explicit Engine();
  ~Engine();

 private:
  std::vector<NativeSymbol> native_symbols_;

  SEEN_DISALLOW_COPY_ASSIGN_AND_MOVE(Engine);
};

}  // namespace seen::runtime

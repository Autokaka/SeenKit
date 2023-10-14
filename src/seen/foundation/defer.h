// Created by Autokaka (qq1909698494@gmail.com) on 2023/10/13.

#pragma once

#include "seen/foundation/class_ext.h"
#include "seen/foundation/types.h"

namespace seen {

class CFDefer {
 public:
  explicit CFDefer(CFClosure callback);
  ~CFDefer();

 private:
  CFClosure callback_;

  DISALLOW_COPY_ASSIGN_AND_MOVE(CFDefer);
};

}  // namespace seen

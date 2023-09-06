// Created by Autokaka (qq1909698494@gmail.com) on 2023/07/30.

#pragma once

#include "seen/foundation/logger.h"

namespace seen {

class CFLogDarwin final : public CFLog {
 protected:
  void Print(const Level& level, const std::string& message) override;
};

}  // namespace seen

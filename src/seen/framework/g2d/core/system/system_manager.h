/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "seen/framework/g2d/core/system/system.h"
#include "seen/framework/g2d/runner/runner.h"

namespace seen::framework::g2d::core {

template <typename R /** Renderer */>
struct ISystemConfig {
  std::vector<std::string> runners;
  std::unordered_map<std::string, ISystemConstructor<R>> systems;
  std::vector<std::string> priority;
};

class SystemManager /** : public EventEmitter */ {
 public:
  std::unordered_map<std::string, runner::Runner> GetRunners() const;

 private:
  std::unordered_map<std::string, IOpaqueSystemPtr> systems_hash_;
};

}  // namespace seen::framework::g2d::core

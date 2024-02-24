// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#pragma once

#include <sol/sol.hpp>

#include "seen/mod/seen.h"

namespace seen::runtime {

mod::Seen::Ptr ExportHostAbilities(sol::state* lua);

}  // namespace seen::runtime

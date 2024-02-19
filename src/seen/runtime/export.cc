// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/runtime/bindings/gpu.h"
#include "seen/runtime/bindings/seen.h"

namespace seen::runtime {

void ExportHostAbilities(sol::state* lua) {
  // seen
  lua->new_usertype<Seen>(                 //
      "seen", sol::no_constructor,         //
      "log", Seen::Log,                    //
      "version", Seen::GetVersion,         //
      "gpu", sol::property(&Seen::GetGPU)  //
  );
  auto seen = lua->get<sol::table>("Seen");
  // GPU
  seen.new_usertype<GPU>(                     //
      "GPU", sol::no_constructor,             //
      "requestAdapter", &GPU::RequestAdapter  //
  );
}

}  // namespace seen::runtime

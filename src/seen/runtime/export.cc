// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/mod/gpu.h"
#include "seen/mod/seen.h"
#include "seen/runtime/glue/gpu.h"

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

void ExportHostAbilities(sol::state* lua) {
  // seen
  lua->new_usertype<Seen>(                 //
      "seen", sol::no_constructor,         //
      "log", Seen::Log,                    //
      "version", Seen::GetVersion,         //
      "gpu", sol::property(&Seen::GetGPU)  //
  );
  auto seen = lua->get<sol::table>("seen");
  // GPU
  seen.new_usertype<GPU>(                                                         //
      "GPU", sol::no_constructor,                                                 //
      "requestAdapter", glue::GPURequestAdapter,                                  //
      "preferredDrawableFormat", sol::property(&GPU::GetPreferredDrawableFormat)  //
  );
}

}  // namespace seen::runtime

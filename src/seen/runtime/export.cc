// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/mod/gpu.h"
#include "seen/mod/object.h"
#include "seen/mod/seen.h"
#include "seen/runtime/glue/gpu.h"

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

void ExportHostAbilities(sol::state* lua) {
  // Seen
  lua->new_usertype<Seen>(                                                //
      "Seen", sol::no_constructor,                                        //
      sol::base_classes, sol::bases<Object>(),                            //
      "log", &Seen::Log,                                                  //
      "version", &Seen::version,                                          //
      "gpu", sol::property(&Seen::GetGPU),                                //
      "framePacer", sol::property(&Seen::GetFramePacer),                  //
      "isRunning", sol::property(&Seen::isRunning),                       //
      "isDrawableAvailable", sol::property(&Seen::IsDrawableAvailable),   //
      "drawableSize", sol::property(&Seen::GetDrawableSize),              //
      "devicePixelRatio", sol::property(&Seen::GetDevicePixelRatio),      //
      "onRunningStateChanged", &Seen::on_running_state_changed_callback,  //
      "onDrawableChanged", &Seen::on_drawable_changed_callback,           //
      "onDrawableSizeChanged", &Seen::on_drawable_size_changed_callback   //
  );
  auto seen = (*lua)["Seen"].get<sol::table>();

  // Seen.Object
  seen.new_usertype<Object>(            //
      "Object", sol::no_constructor,    //
      "className", &Object::class_name  //
  );

  // Seen.FramePacer
  seen.new_usertype<FramePacer>(                                    //
      "FramePacer", sol::no_constructor,                            //
      sol::base_classes, sol::bases<Object>(),                      //
      "requestAnimationFrame", &FramePacer::RequestAnimationFrame,  //
      "cancelAnimationFrame", &FramePacer::CancelAnimationFrame     //
  );

  // Seen.GPU
  seen.new_usertype<GPU>(                        //
      "GPU", sol::no_constructor,                //
      sol::base_classes, sol::bases<Object>(),   //
      "requestAdapter", glue::GPURequestAdapter  //
  );

  // Seen.GPUAdapter
  seen.new_usertype<GPUAdapter>(                                                       //
      "GPUAdapter", sol::no_constructor,                                               //
      sol::base_classes, sol::bases<Object>(),                                         //
      "preferredTextureFormat", sol::property(&GPUAdapter::GetPreferredTextureFormat)  //
  );
}

}  // namespace seen::runtime

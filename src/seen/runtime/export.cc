// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/mod/drawable.h"
#include "seen/mod/gpu.h"
#include "seen/mod/object.h"
#include "seen/mod/seen.h"
#include "seen/runtime/glue/gpu.h"

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

void ExportModules(sol::state* lua) {
  // Seen
  lua->new_usertype<Seen>(                                                //
      "Seen", sol::no_constructor,                                        //
      sol::base_classes, sol::bases<Object>(),                            //
      "log", &Seen::Log,                                                  //
      "version", &Seen::version,                                          //
      "gpu", sol::property(&Seen::GetGPU),                                //
      "framePacer", sol::property(&Seen::GetFramePacer),                  //
      "isRunning", sol::property(&Seen::isRunning),                       //
      "drawable", sol::property(&Seen::GetDrawable),                      //
      "clientSize", sol::property(&Seen::GetClientSize),                  //
      "devicePixelRatio", sol::property(&Seen::GetDevicePixelRatio),      //
      "onRunningStateChanged", &Seen::on_running_state_changed_callback,  //
      "onDrawableChanged", &Seen::on_drawable_changed_callback,           //
      "onClientSizeChanged", &Seen::on_client_size_changed_callback       //
  );
  auto seen = (*lua)["Seen"].get<sol::table>();

  // Seen.Drawable
  seen.new_usertype<Drawable>(                                       //
      "Drawable", sol::no_constructor,                               //
      sol::base_classes, sol::bases<Object>(),                       //
      "size", sol::property(&Drawable::GetSize, &Drawable::SetSize)  //
  );

  // Seen.FramePacer
  seen.new_usertype<FramePacer>(                                    //
      "FramePacer", sol::no_constructor,                            //
      sol::base_classes, sol::bases<Object>(),                      //
      "requestAnimationFrame", &FramePacer::RequestAnimationFrame,  //
      "cancelAnimationFrame", &FramePacer::CancelAnimationFrame     //
  );

  // Seen.GPUAdapter
  seen.new_usertype<GPUAdapter>(               //
      "GPUAdapter", sol::no_constructor,       //
      sol::base_classes, sol::bases<Object>()  //
  );

  // Seen.GPU
  seen.new_usertype<GPU>(                                                        //
      "GPU", sol::no_constructor,                                                //
      sol::base_classes, sol::bases<Object>(),                                   //
      "preferredTextureFormat", sol::property(&GPU::GetPreferredTextureFormat),  //
      "requestAdapter", glue::GPURequestAdapter                                  //
  );

  // Seen.Object
  seen.new_usertype<Object>(            //
      "Object", sol::no_constructor,    //
      "className", &Object::class_name  //
  );
}

}  // namespace seen::runtime

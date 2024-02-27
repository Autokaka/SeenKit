// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/mod/drawable_metrics.h"
#include "seen/mod/gpu.h"
#include "seen/mod/object.h"
#include "seen/mod/seen.h"
#include "seen/runtime/glue/gpu.h"

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

void ExportHostAbilities(sol::state* lua) {
  // Seen
  lua->new_usertype<Seen>(                                                     //
      "Seen", sol::no_constructor,                                             //
      sol::base_classes, sol::bases<Object>(),                                 //
      "log", &Seen::Log,                                                       //
      "version", &Seen::version,                                               //
      "gpu", sol::property(&Seen::GetGPU),                                     //
      "framePacer", sol::property(&Seen::GetFramePacer),                       //
      "isRunning", sol::property(&Seen::isRunning),                            //
      "isDrawableAvailable", sol::property(&Seen::IsDrawableAvailable),        //
      "drawableMetrics", sol::property(&Seen::GetDrawableMetrics),             //
      "onRunningStateChanged", &Seen::on_running_state_changed_callback,       //
      "onDrawableChanged", &Seen::on_drawable_changed_callback,                //
      "onDrawableMetricsChanged", &Seen::on_drawable_metrics_changed_callback  //
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
  auto frame_pacer = (*lua)["Seen"]["FramePacer"].get<sol::table>();

  // Seen.FramePacer.FrameTime
  frame_pacer.new_usertype<FramePacer::FrameTime>(  //
      "FrameTime", sol::no_constructor,             //
      "last", &FramePacer::FrameTime::last,         //
      "now", &FramePacer::FrameTime::now,           //
      "output", &FramePacer::FrameTime::output      //
  );

  // Seen.DrawableMetrics
  seen.new_usertype<DrawableMetrics>(                                 //
      "DrawableMetrics", sol::no_constructor,                         //
      sol::base_classes, sol::bases<Object>(),                        //
      "width", sol::property(&DrawableMetrics::width),                //
      "height", sol::property(&DrawableMetrics::height),              //
      "contentScale", sol::property(&DrawableMetrics::content_scale)  //
  );

  // Seen.GPU
  seen.new_usertype<GPU>(                                                         //
      "GPU", sol::no_constructor,                                                 //
      sol::base_classes, sol::bases<Object>(),                                    //
      "requestAdapter", glue::GPURequestAdapter,                                  //
      "preferredDrawableFormat", sol::property(&GPU::GetPreferredDrawableFormat)  //
  );

  // Seen.GPUAdapter
  seen.new_usertype<GPUAdapter>(               //
      "GPUAdapter", sol::no_constructor,       //
      sol::base_classes, sol::bases<Object>()  //
  );
}

}  // namespace seen::runtime

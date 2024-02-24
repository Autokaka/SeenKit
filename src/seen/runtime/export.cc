// Created by Autokaka (qq1909698494@gmail.com) on 2024/02/18.

#include "seen/runtime/export.h"
#include "seen/mod/drawable_metrics.h"
#include "seen/mod/gpu.h"
#include "seen/runtime/glue/gpu.h"

namespace seen::runtime {

// NOLINTNEXTLINE(google-build-using-namespace)
using namespace mod;

Seen::Ptr ExportHostAbilities(sol::state* lua) {
  // Seen
  lua->new_usertype<Seen>(                                                     //
      "Seen", sol::no_constructor,                                             //
      "log", &Seen::Log,                                                       //
      "version", &Seen::version,                                               //
      "gpu", sol::property(&Seen::GetGPU),                                     //
      "onRunningStateChanged", &Seen::on_running_state_changed_callback,       //
      "onDrawableChanged", &Seen::on_drawable_changed_callback,                //
      "onDrawableMetricsChanged", &Seen::on_drawable_metrics_changed_callback  //
  );
  auto seen = lua->get<sol::table>("Seen");

  // DrawableMetrics
  seen.new_usertype<DrawableMetrics>(                                 //
      "DrawableMetrics", sol::no_constructor,                         //
      "width", sol::property(&DrawableMetrics::width),                //
      "height", sol::property(&DrawableMetrics::height),              //
      "contentScale", sol::property(&DrawableMetrics::content_scale)  //
  );

  // GPU
  seen.new_usertype<GPU>(                                                         //
      "GPU", sol::no_constructor,                                                 //
      "requestAdapter", glue::GPURequestAdapter,                                  //
      "preferredDrawableFormat", sol::property(&GPU::GetPreferredDrawableFormat)  //
  );

  // seen
  auto global = lua->globals();
  auto seen_ptr = std::make_shared<Seen>();
  global["seen"] = seen_ptr;
  return seen_ptr;
}

}  // namespace seen::runtime

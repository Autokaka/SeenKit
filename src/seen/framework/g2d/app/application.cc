/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#include "seen/2d/app/application.h"

namespace seen::g2d {

Application::Application(std::optional<Options> options) {
  if (options) {
    options_ = options.value();
  }
}

}  // namespace seen::g2d
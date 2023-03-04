/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>

namespace seen::g2d {

class Application {
 public:
  struct Options {
    bool antialias;
    float background_alpha = 1;
    std::string background_color = "#000000";
    bool premultiplied_alpha = true;
    glm::vec2 size;
    bool auto_resize = false;
    bool shared_ticker = false;
  };

  Application(std::optional<Options> options = std::nullopt);

  void GetRenderer();
  void ResizeTo(Window window);

 private:
  Options options_;
};

}  // namespace seen::g2d

/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <glm/glm.hpp>
#include <optional>
#include <string>

#include "seen/framework/g2d/core/renderer.h"
#include "seen/framework/g2d/math/shapes/rectangle.h"

namespace seen::framework::g2d {

using ApplicationOptions = core::IRendererOptions;

class Application {
 public:
  Application(std::optional<ApplicationOptions> options = std::nullopt);

  const core::Renderer& GetRenderer();
  void SetRenderer(const core::Renderer& renderer);

  math::Rectangle GetScreen();
  void GetSize();
  void SetSize(glm::vec2 size);

  void GetStage();
  void SetStage();

  void GetTicker();
  void SetTicker();

  void Render();
  void Start();
  void Stop();
  void Destroy(ContainerOptions stage_options);
};

}  // namespace seen::framework::g2d

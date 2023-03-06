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

class Application {
 public:
  using Options = core::IRenderer::Options;

  Application(std::optional<Options> options = std::nullopt);

  const core::RendererPtr& GetRenderer();
  void SetRenderer(const core::RendererPtr& renderer);

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
  void Destroy(Container::Options stage_options);

 private:
  Options options_;
};

}  // namespace seen::framework::g2d

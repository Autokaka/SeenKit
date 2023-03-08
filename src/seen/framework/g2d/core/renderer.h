/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <optional>
#include <string>

#include "seen/framework/g2d/constants/constants.h"
#include "seen/framework/g2d/core/plugin/plugin_system.h"
#include "seen/framework/g2d/core/render_texture/generate_texture_system.h"
#include "seen/framework/g2d/core/render_texture/render_texture.h"
#include "seen/framework/g2d/core/system/system_manager.h"
#include "seen/framework/g2d/math/matrix.h"
#include "seen/framework/g2d/math/shapes/rectangle.h"
#include "seen/framework/g2d/math/transform.h"

namespace seen::framework::g2d::core {

class IRenderableContainer;

class IRenderer;

class IRenderableObject {
  virtual IRenderableContainer GetParent() const = 0;
  virtual void SetParent(IRenderableContainer parent) = 0;
  virtual math::Transform GetTransform() const = 0;
  virtual void SetTransform(math::Transform transform) = 0;

  virtual IRenderableContainer EnableTempParent() const = 0;
  virtual void DisableTempParent(IRenderableContainer parent) = 0;
  virtual void UpdateTransform() = 0;

  virtual void Render(IRenderer renderer) = 0;
};

class IRenderableContainer : public IRenderableObject {
  virtual math::Rectangle GetLocalBounds(std::optional<math::Rectangle> rectangle = std::nullopt,
                                         std::optional<bool> skipChildrenUpdate = std::nullopt) = 0;
};

struct IRendererOptions {
  std::optional<bool> clear_before_render = true;
  std::optional<bool> antialias = true;
  std::optional<bool> premultiplied_alpha = true;

  std::optional<float> background_alpha = 1;
  std::optional<glm::vec4> background_color = glm::vec4(0, 0, 0, 0);

  std::optional<glm::vec2> size = glm::vec2(800, 600);
  std::optional<bool> auto_resize = true;
  std::optional<float> resolution = 1;

  std::optional<bool> shared_ticker = false;
};

struct IRendererRenderOptions {
  std::optional<RenderTexture> renderTexture;
  std::optional<bool> blit;
  std::optional<bool> clear;
  std::optional<math::Matrix> transform;
  std::optional<bool> skipUpdateTransform;
};

class IRenderer {
 public:
  virtual constants::RendererType GetType() const = 0;
  virtual IRendererRenderOptions GetOptions() const = 0;
  virtual std::string GetRendererLogId() const = 0;
  virtual bool RenderToScreen() = 0;
  virtual float GetResolution() const = 0;
  virtual void SetResolution(float resolution) = 0;
  virtual IRenderableObject GetLastObjectRendered() const = 0;
  virtual IRendererPlugins GetPlugins() const = 0;

  virtual glm::vec2 GetSize() const = 0;
  virtual void SetSize(const glm::vec2& size) = 0;

  virtual void Render(IRenderableObject displayObject,
                      std::optional<IRendererRenderOptions> options = std::nullopt) = 0;
  virtual void GenerateTexture(IRenderableObject displayObject,
                               std::optional<IGenerateTextureOptions> options = std::nullopt) = 0;

  virtual void Destroy() = 0;
  virtual void Clear() = 0;
  virtual void Reset() = 0;
};

class Renderer : public SystemManager, IRenderer {
 public:
};

}  // namespace seen::framework::g2d::core

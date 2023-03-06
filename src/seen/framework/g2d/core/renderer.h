/*
 * Created by Autokaka (qq1909698494@gmail.com) on 2023/01/10.
 */

#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "seen/framework/g2d/constants/constants.h"
#include "seen/framework/g2d/core/system/system_manager.h"
#include "seen/framework/g2d/math/shapes/rectangle.h"
#include "seen/framework/g2d/math/transform.h"

namespace seen::framework::g2d::core {

class IRenderableContainer;
using IRenderableContainerPtr = std::shared_ptr<IRenderableContainer>;

class IRenderer;
using IRendererPtr = std::shared_ptr<IRenderer>;

class IRenderableObject {
  virtual IRenderableContainerPtr GetParent() const = 0;
  virtual void SetParent(IRenderableContainerPtr parent) = 0;
  virtual math::TransformPtr GetTransform() const = 0;
  virtual void SetTransform(math::TransformPtr transform) = 0;

  virtual IRenderableContainerPtr EnableTempParent() const = 0;
  virtual void DisableTempParent(IRenderableContainerPtr parent) = 0;
  virtual void UpdateTransform() = 0;

  virtual void Render(IRendererPtr renderer) = 0;
};
using IRenderableObjectPtr = std::shared_ptr<IRenderableObject>;

class IRenderableContainer : public IRenderableObject {
  virtual math::RectanglePtr GetLocalBounds(math::RectanglePtr rectangle = nullptr,
                                            bool skipChildrenUpdate = false) = 0;
};

class IRenderer {
 public:
  struct Options {
    bool clear_before_render = true;
    bool antialias = true;
    bool premultiplied_alpha = true;

    float background_alpha = 1;
    glm::vec4 background_color = glm::vec4(0, 0, 0, 0);

    glm::vec2 size = glm::vec2(800, 600);
    bool auto_resize = true;
    float resolution = 1;

    bool shared_ticker = false;
  };

  virtual constants::RendererType GetType() const = 0;
  virtual Options GetOptions() const = 0;
  virtual std::string GetRendererLogId() const = 0;
  virtual bool RenderToScreen() = 0;
  virtual float GetResolution() const = 0;
  virtual void SetResolution(float resolution) = 0;
  virtual void GetLastObjectRendered() const = 0;
  virtual void GetPlugins() const = 0;

  virtual glm::vec2 GetSize() const = 0;
  virtual void SetSize(const glm::vec2& size) = 0;

  virtual void Render() = 0;
  virtual void GenerateTexture() = 0;

  virtual void Destroy() = 0;
  virtual void Clear() = 0;
  virtual void Reset() = 0;
};

class Renderer : public SystemManager, IRenderer {
 public:
};

using RendererPtr = std::shared_ptr<Renderer>;

}  // namespace seen::framework::g2d::core

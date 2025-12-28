#ifndef YUGGOTH_RENDERER_H
#define YUGGOTH_RENDERER_H

#include "renderer_context.h"
#include "base_renderer_backend.h"
#include "yuggoth/graphics/descriptors/descriptor_pool.h"
#include "yuggoth/graphics/descriptors/descriptor_set.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics/image/image2D.h"
#include "yuggoth/graphics/image/image_depth.h"

namespace Yuggoth {

class Renderer {
public:
  Renderer(const RendererContext &renderer_context);

  const RendererContext &GetRendererConstext() const;

  const Image2D &GetImage() const;

  void OnViewportResize(uint32_t width, uint32_t height);

  void Begin(Scene *scene);

  void Draw(Scene *scene);

  void End();

protected:
  void Create();

private:
  RendererContext renderer_context_;
  DescriptorPool descriptor_pool_;
  DescriptorSet descriptor_set_;
  CommandBuffer command_buffer_;
  Image2D target_image_;
  ImageDepth depth_image_;
  BaseRendererBackend base_renderer_backend_;
};

} // namespace Yuggoth

#endif // YUGGOTH_RENDERER_H
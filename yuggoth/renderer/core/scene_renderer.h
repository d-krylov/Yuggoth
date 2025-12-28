#ifndef YUGGOTH_SCENE_RENDERER_H
#define YUGGOTH_SCENE_RENDERER_H

#include "yuggoth/graphics/core/graphics_common.h"
#include "renderer_context.h"

namespace Yuggoth {

class Scene;

class ResourceOwningModel;

class SceneRenderer {
public:
  SceneRenderer(const RendererContext &renderer_context);

  void DirectDraw(Scene *scene);
  void IndirectDraw(Scene *scene);

protected:
  void DrawMeshlets(Scene *scene);

private:
  Image2D target_image_;
  ImageDepth depth_image_;
  CommandBuffer command_buffer_;
  std::vector<DrawIndexedIndirectCommand> commands_;
  Buffer indirect_buffer_;
  Buffer transform_buffer_;
  RendererContext renderer_context_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_RENDERER_H
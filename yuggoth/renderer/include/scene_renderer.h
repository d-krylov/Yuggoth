#ifndef YUGGOTH_SCENE_RENDERER_H
#define YUGGOTH_SCENE_RENDERER_H

#include "yuggoth/graphics/core/graphics_common.h"

namespace Yuggoth {

class Scene;

class SceneRenderer {
public:
  SceneRenderer();

  const Image2D &GetImage() const;

  void Begin(Scene *scene);
  void End();

  void Draw();

  void OnViewportResize(uint32_t width, uint32_t height);

protected:
  void Initialize();

private:
  Image2D target_image_;
  ImageDepth depth_image_;
  CommandBuffer command_buffer_;
  GraphicsPipeline graphics_pipeline_;
  std::vector<DrawIndexedIndirectCommand> commands_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_RENDERER_H
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

  void Draw(CommandBuffer &command_buffer);

  void OnViewportResize(uint32_t width, uint32_t height);

protected:
  void Initialize();

private:
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  Buffer scratch_buffer_;
  Image2D target_image_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_RENDERER_H
#ifndef YUGGOTH_SCENE_RENDERER_H
#define YUGGOTH_SCENE_RENDERER_H

#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/image/image2D.h"
#include "yuggoth/graphics/command/command_buffer.h"

namespace Yuggoth {

class SceneRenderer {
public:
  SceneRenderer();

  const Image2D &GetImage() const;

  void Draw(CommandBuffer &command_buffer);

  void OnViewportResize(uint32_t width, uint32_t height);

protected:
  void Initialize();

private:
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  Image2D target_image_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_RENDERER_H
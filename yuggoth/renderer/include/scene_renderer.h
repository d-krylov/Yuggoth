#ifndef YUGGOTH_SCENE_RENDERER_H
#define YUGGOTH_SCENE_RENDERER_H

#include "yuggoth/graphics/core/graphics_common.h"

namespace Yuggoth {

class Scene;
class BufferManager;
class ResourceOwningModel;

class SceneRenderer {
public:
  SceneRenderer(BufferManager *buffer_manager);

  const Image2D &GetImage() const;

  void Begin(Scene *scene);
  void End();

  void Draw(Scene *scene);

  void DirectDraw(Scene *scene);
  void IndirectDraw(Scene *scene);

  void OnViewportResize(uint32_t width, uint32_t height);

protected:
  void Initialize();

  void DrawResourceOwningModel(const ResourceOwningModel &model);

private:
  Image2D target_image_;
  ImageDepth depth_image_;
  CommandBuffer command_buffer_;
  GraphicsPipeline graphics_pipeline_;
  GraphicsPipeline indirect_pipeline_;
  GraphicsPipeline mesh_pipeline_;
  std::vector<DrawIndexedIndirectCommand> commands_;
  Buffer indirect_buffer_;
  Buffer transform_buffer_;
  BufferManager *buffer_manager_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_RENDERER_H
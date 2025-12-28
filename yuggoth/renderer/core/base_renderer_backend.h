#ifndef YUGGOTH_BASE_RENDERER_BACKEND_H
#define YUGGOTH_BASE_RENDERER_BACKEND_H

namespace Yuggoth {

class Scene;
class Camera;
class Renderer;
class CommandBuffer;
class GraphicsPipeline;
class ResourceOwningModel;
struct IndexedIndirectCommand;

enum class ObjectMode;

class BaseRendererBackend {
public:
  BaseRendererBackend(Renderer *renderer);

  void DrawDirect(CommandBuffer *command_buffer, Scene *scene, const Camera *camera, ObjectMode object_mode);

  void DrawIndirect(CommandBuffer *command_buffer, Scene *scene, const Camera *camera, ObjectMode object_mode);

protected:
  void DrawDirectColored(CommandBuffer *command_buffer, const GraphicsPipeline *pipeline, const ResourceOwningModel *model);
  void DrawDirectTextured(CommandBuffer *command_buffer, const GraphicsPipeline *pipeline, const ResourceOwningModel *model);

private:
  Renderer *renderer_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_BASE_RENDERER_BACKEND_H
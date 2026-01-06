#ifndef YUGGOTH_RAY_TRACE_RENDERER_BACKEND_H
#define YUGGOTH_RAY_TRACE_RENDERER_BACKEND_H

#include "yuggoth/graphics/acceleration/shader_binding_table.h"

namespace Yuggoth {

class Renderer;
class CommandBuffer;
class Scene;

class RaytraceRendererBackend {
public:
  RaytraceRendererBackend(Renderer *renderer);

  void Draw(CommandBuffer *command_buffer, Scene *scene, uint32_t width, uint32_t height);

private:
  Renderer *renderer_ = nullptr;
  ShaderBindingTable shader_binding_table_;
};

} // namespace Yuggoth

#endif // YUGGOTH_RAY_TRACE_RENDERER_BACKEND_H
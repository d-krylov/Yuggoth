#ifndef YUGGOTH_MESH_RENDERER_BACKEND_H
#define YUGGOTH_MESH_RENDERER_BACKEND_H

namespace Yuggoth {

class Renderer;

class MeshRendererBackend {
public:
  MeshRendererBackend(Renderer *renderer);

  void DrawDirect();

  void DrawIndirect();

private:
  Renderer *renderer_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_MESH_RENDERER_BACKEND_H
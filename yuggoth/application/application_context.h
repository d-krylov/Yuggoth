#ifndef YUGGOTH_APPLICATION_CONTEXT_H
#define YUGGOTH_APPLICATION_CONTEXT_H

namespace Yuggoth {

class SceneManager;
class AssetManager;
class MaterialManager;
class Renderer;
class PipelineLibrary;
class ShaderLibrary;
class GraphicsDevice;

struct EditorContext {
  SceneManager *scene_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
  ShaderLibrary *shader_library_ = nullptr;
  GraphicsDevice *graphics_device = nullptr;
  Renderer *renderer_ = nullptr;
};

struct RendererContext {
  PipelineLibrary *pipeline_library_ = nullptr;
  GraphicsDevice *graphics_device_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
};

struct AssetManagerCreateContext {
  GraphicsDevice *graphics_device_ = nullptr;
  MaterialManager *material_manager_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_APPLICATION_CONTEXT_H
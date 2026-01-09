#ifndef YUGGOTH_APPLICATION_CONTEXT_H
#define YUGGOTH_APPLICATION_CONTEXT_H

namespace Yuggoth {

class SceneManager;
class AssetManager;
class MaterialManager;
class BufferManager;
class Renderer;
class PipelineLibrary;
class ShaderLibrary;
class RendererResources;

struct EditorContext {
  SceneManager *scene_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
  BufferManager *buffer_manager_ = nullptr;
  ShaderLibrary *shader_library_ = nullptr;
  RendererResources *renderer_resources_ = nullptr;
  Renderer *renderer_ = nullptr;
};

struct RendererContext {
  PipelineLibrary *pipeline_library_ = nullptr;
  BufferManager *buffer_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
};

struct AssetManagerCreateContext {
  BufferManager *buffer_manager_ = nullptr;
  MaterialManager *material_manager_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_APPLICATION_CONTEXT_H
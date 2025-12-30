#ifndef YUGGOTH_APPLICATION_CONTEXT_H
#define YUGGOTH_APPLICATION_CONTEXT_H

namespace Yuggoth {

class SceneManager;
class AssetManager;
class BufferManager;
class Renderer;
class PipelineLibrary;

struct EditorContext {
  SceneManager *scene_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
  BufferManager *buffer_manager_ = nullptr;
  Renderer *renderer_ = nullptr;
};

struct RendererContext {
  PipelineLibrary *pipeline_library_ = nullptr;
  BufferManager *buffer_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_APPLICATION_CONTEXT_H
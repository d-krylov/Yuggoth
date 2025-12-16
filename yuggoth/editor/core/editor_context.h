#ifndef YUGGOTH_EDITOR_CONTEXT_H
#define YUGGOTH_EDITOR_CONTEXT_H

#include "yuggoth/scene/core/scene_manager.h"
#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/renderer/include/scene_renderer.h"

namespace Yuggoth {

struct EditorContext {
  SceneManager *scene_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
  SceneRenderer *scene_renderer_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_EDITOR_CONTEXT_H
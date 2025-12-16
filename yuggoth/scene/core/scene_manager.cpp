#include "scene_manager.h"
#include "yuggoth/asset/include/asset_manager.h"
#include <cassert>

namespace Yuggoth {

SceneManager::SceneManager(AssetManager *asset_manager) : asset_manager_(asset_manager) {
}

void SceneManager::EnqueueScene() {
  SceneContext scene_context;
  scene_context.asset_manager_ = asset_manager_;
  scene_context.scene_manager_ = this;
  auto scene = std::make_unique<Scene>(scene_context);
  current_scene_ = scene.get();
  scenes_.emplace_back(std::move(scene));
}

Scene *SceneManager::GetCurrentScene() const {
  assert(current_scene_ != nullptr);
  return current_scene_;
}

bool SceneManager::HasValidScenes() const {
  return current_scene_ != nullptr;
}

} // namespace Yuggoth
#include "scene_manager.h"
#include <cassert>

namespace Yuggoth {

void SceneManager::EnqueueScene() {
  auto scene = std::make_unique<Scene>();
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
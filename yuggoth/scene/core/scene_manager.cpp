#include "scene_manager.h"
#include "yuggoth/asset/core/asset_manager.h"
#include <ranges>
#include <cassert>

namespace Yuggoth {

SceneManager::SceneManager(AssetManager *asset_manager) : asset_manager_(asset_manager) {
}

void SceneManager::EnqueueScene() {
  SceneContext scene_context;
  scene_context.asset_manager_ = asset_manager_;
  scene_context.scene_manager_ = this;
  auto scene_name = std::format("Scene {}", GetNumberScenes());
  auto scene = std::make_unique<Scene>(scene_context, scene_name);
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

std::size_t SceneManager::GetNumberScenes() const {
  return scenes_.size();
}

const Scene *SceneManager::GetSceneWithIndex(uint32_t index) const {
  return scenes_[index].get();
}

void SceneManager::SwitchScene(uint32_t scene_index) {
  current_scene_ = scenes_[scene_index].get();
}

void SceneManager::SwitchScene(std::string_view scene_name) {
}

} // namespace Yuggoth
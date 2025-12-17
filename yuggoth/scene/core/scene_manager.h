#ifndef YUGGOTH_SCENE_MANAGER_H
#define YUGGOTH_SCENE_MANAGER_H

#include "scene.h"
#include <memory>

namespace Yuggoth {

class AssetManager;

class SceneManager {
public:
  SceneManager(AssetManager *asset_manager);

  void EnqueueScene();

  Scene *GetCurrentScene() const;

  bool HasValidScenes() const;

  std::size_t GetNumberScenes() const;

  const Scene *GetSceneWithIndex(uint32_t index) const;

  void SwitchScene(uint32_t scene_index);

  void SwitchScene(std::string_view scene_name);

private:
  std::vector<std::unique_ptr<Scene>> scenes_;
  AssetManager *asset_manager_ = nullptr;
  Scene *current_scene_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_MANAGER_H

#ifndef YUGGOTH_SCENE_MANAGER_H
#define YUGGOTH_SCENE_MANAGER_H

#include "scene.h"
#include <memory>

namespace Yuggoth {

class SceneManager {
public:
  void EnqueueScene();

  Scene *GetCurrentScene() const;

  bool HasValidScenes() const;

private:
  std::vector<std::unique_ptr<Scene>> scenes_;
  Scene *current_scene_{nullptr};
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_MANAGER_H

#ifndef YUGGOTH_SCENE_H
#define YUGGOTH_SCENE_H

#include <entt/entity/registry.hpp>
#include "yuggoth/scene/components/components.h"

namespace Yuggoth {

class Entity;
class SceneManager;
class AssetManager;

struct SceneContext {
  SceneManager *scene_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
};

class Scene {
public:
  Scene(const SceneContext &scene_context, std::string_view name = "Scene");

  Entity CreateEntity();
  Entity CreateEntityWithName(std::string_view name);

  void DestroyEntity(Entity entity);
  void SetCurrentCamera(Camera *camera);
  void SetName(std::string_view name);

  entt::registry &GetRegistry();

  std::string_view GetName() const;

  Camera *GetCurrentCamera();

private:
  entt::registry registry_;
  SceneManager *scene_manager_ = nullptr;
  AssetManager *asset_manager_ = nullptr;
  std::string scene_name_;
  Camera *current_camera_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_H
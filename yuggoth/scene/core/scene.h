#ifndef YUGGOTH_SCENE_H
#define YUGGOTH_SCENE_H

#include <entt/entity/registry.hpp>
#include "yuggoth/scene/components/components.h"

namespace Yuggoth {

class Entity;

class Scene {
public:
  Scene();

  entt::registry &GetRegistry();

  Entity CreateEntity();
  Entity CreateEntityWithName(std::string_view name);
  Entity GetEntityByUUID(UUID id);

  void DestroyEntity(Entity entity);

  void SetCurrentCamera(Camera *camera);

  Camera *GetCurrentCamera();

private:
  entt::registry registry_;
  Camera *current_camera_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_H
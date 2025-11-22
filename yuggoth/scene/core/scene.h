#ifndef YUGGOTH_SCENE_H
#define YUGGOTH_SCENE_H

#include <entt/entity/registry.hpp>
#include "yuggoth/core/include/uuid.h"

namespace Yuggoth {

class Entity;

class Scene {
public:
  Scene();

  entt::registry &GetRegistry();

  Entity CreateEntity();
  Entity CreateEntityWithName(std::string_view name);

  Entity GetEntityByUUID(UUID id);

private:
  entt::registry registry_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SCENE_H
#include "scene.h"
#include "entity.h"
#include "yuggoth/scene/components/components.h"

namespace Yuggoth {

Scene::Scene() {
}

entt::registry &Scene::GetRegistry() {
  return registry_;
}

Entity Scene::CreateEntity() {
}

Entity Scene::CreateEntityWithName(std::string_view name) {
  auto &registry = GetRegistry();
  auto entity = registry.create();
  registry.emplace<NameComponent>(entity, name);
  return Entity(entity, this);
}

Entity Scene::GetEntityByUUID(UUID id) {
}

} // namespace Yuggoth
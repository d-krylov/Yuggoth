#include "scene.h"
#include "entity.h"
#include "scene_manager.h"
#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/scene/components/components.h"

namespace Yuggoth {

Scene::Scene(const SceneContext &scene_context) : scene_manager_(scene_context.scene_manager_), asset_manager_(scene_context.asset_manager_) {
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
  registry.emplace<UUIDComponent>(entity);
  return Entity(entity, this);
}

void Scene::DestroyEntity(Entity entity) {
  auto &registry = GetRegistry();
  registry.destroy(entity.GetHandle());
}

void Scene::SetName(std::string_view name) {
  scene_name_ = name;
}

void Scene::SetCurrentCamera(Camera *camera) {
  current_camera_ = camera;
}

Camera *Scene::GetCurrentCamera() {
  return current_camera_;
}

} // namespace Yuggoth
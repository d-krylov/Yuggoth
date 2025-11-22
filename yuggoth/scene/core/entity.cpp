#include "entity.h"

namespace Yuggoth {

Entity::Entity(entt::entity entity, Scene *scene) : entity_(entity), scene_(scene) {
}

Scene *Entity::GetScene() const {
  return scene_;
}

entt::entity Entity::GetHandle() const {
  return entity_;
}

} // namespace Yuggoth
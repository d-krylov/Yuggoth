#ifndef YUGGOTH_ENTITY_H
#define YUGGOTH_ENTITY_H

#include "scene.h"

namespace Yuggoth {

class Entity {
public:
  Entity(entt::entity entity = entt::null, Scene *scene = nullptr);

  Scene *GetScene() const;
  entt::entity GetHandle() const;

  template <typename T, typename... Arguments> T &AddComponent(Arguments &&...arguments);

  template <typename Component> Component &GetComponent() const;

  template <typename Component> Component *TryGetComponent() const;

  template <typename Component> bool HasComponent() const;

  bool operator==(const Entity &other) const = default;

private:
  entt::entity entity_ = entt::null;
  Scene *scene_ = nullptr;
};

} // namespace Yuggoth

#include "entity.ipp"

#endif // YUGGOTH_ENTITY_H

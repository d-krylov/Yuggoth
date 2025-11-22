#ifndef YUGGOTH_ENTITY_IPP
#define YUGGOTH_ENTITY_IPP

namespace Yuggoth {

template <typename T, typename... Arguments>
T &Entity::AddComponent(Arguments &&...arguments) {
  return GetScene()->GetRegistry().emplace<T>(GetHandle(), std::forward<Arguments>(arguments)...);
}

template <typename Component>
Component &Entity::GetComponent() {
  return GetScene()->GetRegistry().get<Component>(GetHandle());
}

template <typename Component>
Component *Entity::TryGetComponent() {
  return GetScene()->GetRegistry().try_get<Component>(GetHandle());
}

} // namespace Yuggoth

#endif // YUGGOTH_ENTITY_IPP
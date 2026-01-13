#ifndef YUGGOTH_COMPONENTS_H
#define YUGGOTH_COMPONENTS_H

#include <string>
#include "camera.h"
#include "light.h"
#include "model_component.h"
#include "yuggoth/core/tools/uuid.h"
#include "yuggoth/mathematics/include/transform.h"
#include <entt/entity/fwd.hpp>

namespace Yuggoth {

struct NameComponent {
  NameComponent(std::string_view name) : name_(name) {
  }

  std::string name_;
};

struct UUIDComponent {
  UUID uuid_;
};

struct HierarchyComponent {
  entt::entity parent_;
  entt::entity next_;
  entt::entity previous_;
  entt::entity begin_;
};

} // namespace Yuggoth

#endif // YUGGOTH_COMPONENTS_H

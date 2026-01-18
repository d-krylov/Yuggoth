#ifndef YUGGOTH_COMPONENTS_H
#define YUGGOTH_COMPONENTS_H

#include <string>
#include "camera.h"
#include "light.h"
#include "yuggoth/core/tools/uuid.h"
#include "yuggoth/mathematics/include/transform.h"
#include <yuggoth/scene/world/terrain.h>
#include "yuggoth/asset/model/model.h"
#include "yuggoth/core/intrusive/intrusive_pointer.h"
#include <memory>
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

struct ModelComponent {
  IntrusivePointer<Model> model_;
};

struct TerrainComponent {
  IntrusivePointer<Terrain> terrain_;
};

} // namespace Yuggoth

#endif // YUGGOTH_COMPONENTS_H

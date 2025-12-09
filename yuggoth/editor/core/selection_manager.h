#ifndef YUGGOTH_SELECTION_MANAGER_H
#define YUGGOTH_SELECTION_MANAGER_H

#include "yuggoth/scene/core/entity.h"

namespace Yuggoth {

class SelectionManager {
public:
  void SetSelected(Entity entity);
  void Deselect(Entity entity);
  void Clear();

  bool IsEntitySelected(Entity entity) const;

  std::span<const Entity> GetSelectedEntities() const;

private:
  std::vector<Entity> entities_;
};

} // namespace Yuggoth

#endif // YUGGOTH_SELECTION_MANAGER_H
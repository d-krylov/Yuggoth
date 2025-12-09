#include "selection_manager.h"
#include <ranges>

namespace Yuggoth {

void SelectionManager::SetSelected(Entity entity) {
  auto iterator = std::ranges::find(entities_, entity);
  if (iterator == entities_.end()) {
    entities_.emplace_back(entity);
  }
}

void SelectionManager::Deselect(Entity entity) {
  auto iterator = std::ranges::find(entities_, entity);
  if (iterator != entities_.end()) {
    entities_.erase(iterator);
  }
}

void SelectionManager::Clear() {
  entities_.clear();
}

bool SelectionManager::IsEntitySelected(Entity entity) const {
  return std::ranges::contains(entities_, entity);
}

std::span<const Entity> SelectionManager::GetSelectedEntities() const {
  return entities_;
}

} // namespace Yuggoth
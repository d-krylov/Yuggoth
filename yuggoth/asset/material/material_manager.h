#ifndef YUGGOTH_MATERIAL_MANAGER_H
#define YUGGOTH_MATERIAL_MANAGER_H

#include <vector>
#include "yuggoth/core/tools/include/uuid.h"

namespace Yuggoth {

class MaterialManager {
public:
private:
  std::vector<UUID> asset_uuids_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MATERIAL_MANAGER_H
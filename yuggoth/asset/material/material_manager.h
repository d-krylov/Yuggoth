#ifndef YUGGOTH_MATERIAL_MANAGER_H
#define YUGGOTH_MATERIAL_MANAGER_H

#include <vector>
#include "yuggoth/core/tools/uuid.h"
#include "yuggoth/graphics/descriptors/descriptor_set.h"
#include "yuggoth/graphics/descriptors/descriptor_pool.h"

namespace Yuggoth {

class AssetManager;

class MaterialManager {
public:
  MaterialManager(AssetManager *asset_manager);

  void UpdateAssets();

protected:
  void CreateDescriptorSet();

private:
  DescriptorPool descriptor_pool_;
  DescriptorSet descriptor_set_;
  std::vector<UUID> asset_uuids_;
  AssetManager *asset_manager_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_MATERIAL_MANAGER_H
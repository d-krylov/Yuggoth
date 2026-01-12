#include "material_manager.h"
#include "yuggoth/asset/core/asset_manager.h"

namespace Yuggoth {

MaterialManager::MaterialManager(AssetManager *asset_manager) {
}

void MaterialManager::UpdateAssets() {
  const auto &assets = asset_manager_->GetAssets();
  for (const auto &[uuid, asset] : assets) {
    if (asset->GetAssetKind() == AssetKind::IMAGE) {
      auto image = static_pointer_cast<Image>(asset);
    }
  }
}

void MaterialManager::CreateDescriptorSet() {
  std::array<Walle::DescriptorPoolSize, 1> descriptor_pool_sizes;
  descriptor_pool_sizes[0].descriptorCount = 2;
  descriptor_pool_sizes[0].type = Walle::DescriptorType::E_COMBINED_IMAGE_SAMPLER;
  descriptor_pool_ = DescriptorPool(descriptor_pool_sizes, Walle::DescriptorPoolCreateMaskBits::E_UPDATE_AFTER_BIND_BIT, 1);
  // descriptor_set_ = DescriptorSet(descriptor_pool_.GetHandle());
}

} // namespace Yuggoth
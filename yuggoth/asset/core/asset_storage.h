#ifndef YUGGOTH_ASSET_STORAGE_H
#define YUGGOTH_ASSET_STORAGE_H

#include "yuggoth/asset/model/model_storage.h"
#include "yuggoth/core/tools/include/uuid.h"
#include "yuggoth/graphics_device/include/common_types.h"
#include "ankerl/unordered_dense.h"
#include <memory>

namespace Yuggoth {

struct AssetInformation {
  std::filesystem::path path_;
  std::filesystem::file_time_type time_;
};

class AssetStorage {
public:
  void AddModelStorage(UUID uuid, ModelStorage model_loader);

  const ModelStorage &GetModelStorage(UUID uuid) const;

private:
  ankerl::unordered_dense::map<UUID, ModelStorage> model_storages_;
  ankerl::unordered_dense::map<UUID, AssetInformation> asset_information_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_STORAGE_H
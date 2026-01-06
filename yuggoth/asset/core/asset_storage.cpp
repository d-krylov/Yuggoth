#include "asset_storage.h"
#include "yuggoth/asset/model/model_storage.h"
#include "yuggoth/graphics/command/command_buffer.h"

namespace Yuggoth {

void AssetStorage::AddModelStorage(UUID uuid, ModelStorage model_loader) {
  model_storages_.emplace(uuid, std::move(model_loader));
}

const ModelStorage &AssetStorage::GetModelStorage(UUID uuid) const {
  return model_storages_.at(uuid);
}

} // namespace Yuggoth

#include "asset_registry.h"

namespace Yuggoth {

UUID AssetRegistry::GetDiskAssetUUID(const std::filesystem::path &path) const {
  return path_uuid_map_.at(path);
}

const std::filesystem::path &AssetRegistry::GetDiskAssetPath(UUID uuid) const {
  return uuid_path_map_.at(uuid);
}

} // namespace Yuggoth
#ifndef YUGGOTH_ASSET_REGISTRY_H
#define YUGGOTH_ASSET_REGISTRY_H

#include "ankerl/unordered_dense.h"
#include "yuggoth/core/tools/uuid.h"
#include <filesystem>

namespace Yuggoth {

class AssetRegistry {
public:
  UUID GetDiskAssetUUID(const std::filesystem::path &path) const;
  const std::filesystem::path &GetDiskAssetPath(UUID uuid) const;

private:
  ankerl::unordered_dense::map<std::filesystem::path, UUID> path_uuid_map_;
  ankerl::unordered_dense::map<UUID, std::filesystem::path> uuid_path_map_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_REGISTRY_H
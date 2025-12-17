#ifndef YUGGOTH_ASSET_MANAGER_H
#define YUGGOTH_ASSET_MANAGER_H

#include "asset.h"
#include <memory>
#include <unordered_map>

namespace Yuggoth {

class BufferManager;

class Model;
class BufferOwningModel;

class AssetManager {
public:
  AssetManager(BufferManager *buffer_manager);

  std::shared_ptr<BufferOwningModel> RegisterModel(const std::filesystem::path &path);

private:
  BufferManager *buffer_manager_;
  std::unordered_map<UUID, std::shared_ptr<Asset>> assets_;
  std::unordered_map<std::filesystem::path, UUID> uuids_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_MANAGER_H
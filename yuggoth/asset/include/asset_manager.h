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

  std::shared_ptr<BufferOwningModel> RegisterBufferOwningModel(const std::filesystem::path &path);

  std::shared_ptr<Model> RegisterModel(const std::filesystem::path &path);

  void Update();

private:
  BufferManager *buffer_manager_ = nullptr;
  std::unordered_map<UUID, std::weak_ptr<Asset>> assets_;
  std::unordered_map<std::filesystem::path, UUID> uuids_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_MANAGER_H
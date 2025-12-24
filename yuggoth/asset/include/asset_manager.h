#ifndef YUGGOTH_ASSET_MANAGER_H
#define YUGGOTH_ASSET_MANAGER_H

#include "asset.h"
#include "yuggoth/core/intrusive/intrusive_pointer.h"
#include "yuggoth/graphics/image/image2D.h"
#include <memory>
#include <unordered_map>

namespace Yuggoth {

class BufferManager;

class Model;
class ResourceOwningModel;

class AssetManager {
public:
  AssetManager(BufferManager *buffer_manager);

  IntrusivePointer<ResourceOwningModel> RegisterResourceOwningModel(const std::filesystem::path &path);

  IntrusivePointer<Model> RegisterModel(const std::filesystem::path &path);

  IntrusivePointer<Image2D> RegisterTexture(const std::filesystem::path &path);

  void Update();

private:
  BufferManager *buffer_manager_ = nullptr;
  std::unordered_map<UUID, IntrusivePointer<Asset>> assets_;
  std::unordered_map<std::filesystem::path, UUID> uuids_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_MANAGER_H
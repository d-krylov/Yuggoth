#ifndef YUGGOTH_ASSET_MANAGER_H
#define YUGGOTH_ASSET_MANAGER_H

#include "asset.h"
#include "primitives.h"
#include "yuggoth/core/intrusive/intrusive_pointer.h"
#include "yuggoth/graphics/image/image2D.h"
#include <memory>
#include "asset_storage.h"

namespace Yuggoth {

class BufferManager;
class Model;

class AssetManager {
public:
  AssetManager(BufferManager *buffer_manager);

  IntrusivePointer<Model> RegisterModel(const std::filesystem::path &path);
  IntrusivePointer<Model> RegisterModel(PrimitiveKind primitive_kind);

  IntrusivePointer<Image2D> RegisterTexture(const std::filesystem::path &path);

  void Update();

  const AssetStorage &GetAssetStorage() const;

private:
  AssetStorage asset_storage_;
  std::unordered_map<UUID, IntrusivePointer<Asset>> assets_;
  std::unordered_map<std::filesystem::path, UUID> outer_uuids_;
  std::unordered_map<PrimitiveKind, UUID> primitives_uuids_;
  BufferManager *buffer_manager_ = nullptr;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_MANAGER_H
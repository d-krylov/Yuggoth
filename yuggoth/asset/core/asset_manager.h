#ifndef YUGGOTH_ASSET_MANAGER_H
#define YUGGOTH_ASSET_MANAGER_H

#include "asset.h"
#include "yuggoth/asset/model/primitives.h"
#include "yuggoth/core/intrusive/intrusive_pointer.h"
#include "yuggoth/application/application_context.h"
#include "yuggoth/graphics/image/image2D.h"
#include <memory>
#include "asset_storage.h"

namespace Yuggoth {

class Model;

using MeshBufferRange = std::pair<BufferRange, BufferRange>;

class AssetManager {
public:
  AssetManager(const AssetManagerCreateContext &create_context);

  IntrusivePointer<Model> RegisterModel(const std::filesystem::path &path);
  IntrusivePointer<Model> RegisterModel(PrimitiveKind primitive_kind);

  IntrusivePointer<Image2D> RegisterTexture(const std::filesystem::path &path);

  void Update();

  const AssetStorage &GetAssetStorage() const;

protected:
  MeshBufferRange PutMeshDataInBuffers(const MeshData &mesh_data);

private:
  AssetManagerCreateContext create_context_;
  AssetStorage asset_storage_;
  std::unordered_map<UUID, IntrusivePointer<Asset>> assets_;
  std::unordered_map<std::filesystem::path, UUID> outer_uuids_;
  std::unordered_map<PrimitiveKind, UUID> primitives_uuids_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_MANAGER_H
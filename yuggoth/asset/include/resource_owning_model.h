#ifndef YUGGOTH_RESOURCE_OWNING_MODEL_H
#define YUGGOTH_RESOURCE_OWNING_MODEL_H

#include "asset.h"
#include "mesh.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/image/image2D.h"

namespace Yuggoth {

class BottomLevelGeometry;

class ResourceOwningModel : public Asset {
public:
  const Buffer &GetVertexBuffer() const;
  const Buffer &GetIndexBuffer() const;
  const Buffer &GetMeshBuffer() const;
  const Buffer &GetMeshletBuffer() const;

  std::span<const Mesh> GetMeshes() const;
  std::span<const Image2D> GetImages() const;

  void SetMeshlets(std::span<const Meshlet> meshlets);
  void SetMeshes(std::span<const Mesh> meshes);
  void SetVertices(std::span<const Vertex> vertices);
  void SetIndices(std::span<const uint32_t> indices);

  void SetImages(std::span<const std::filesystem::path> pathes);

  BottomLevelGeometry GetBottomLevelGeometry() const;

  AssetKind GetAssetKind() const override;

private:
  Buffer vertex_buffer_;
  Buffer index_buffer_;
  Buffer mesh_buffer_;
  Buffer meshlet_buffer_;
  std::vector<Mesh> meshes_;
  std::vector<Image2D> images_;
};

} // namespace Yuggoth

#endif // YUGGOTH_RESOURCE_OWNING_MODEL_H

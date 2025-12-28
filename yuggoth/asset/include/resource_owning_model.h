#ifndef YUGGOTH_RESOURCE_OWNING_MODEL_H
#define YUGGOTH_RESOURCE_OWNING_MODEL_H

#include "model_base.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/image/image2D.h"

namespace Yuggoth {

class BottomLevelGeometry;

class ResourceOwningModel : public ModelBase {
public:
  BufferRange GetVertexBufferRange() const override;
  BufferRange GetIndexBufferRange() const override;
  BufferRange GetMeshBufferRange() const override;
  BufferRange GetMeshletBufferRange() const override;

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

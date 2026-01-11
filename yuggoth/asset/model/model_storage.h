#ifndef YUGGOTH_MODEL_STORAGE_H
#define YUGGOTH_MODEL_STORAGE_H

#include "mesh.h"
#include "yuggoth/asset/material/material.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include <filesystem>
#include <vector>

namespace Yuggoth {

class ModelStorage {
public:
  MeshData Load(const std::filesystem::path &path);

  std::span<const Mesh> GetMeshes() const;
  std::span<const Meshlet> GetMeshlets() const;
  std::span<const Texture> GetTextures() const;
  std::span<const Material> GetMaterials() const;
  std::span<const std::filesystem::path> GetImagePaths() const;

  std::size_t GetNumberOfVertices() const;
  std::size_t GetNumberOfIndices() const;

  void BuildMeshlets(std::span<const Vector3f> vertices, std::span<const uint32_t> indices);

protected:
  MeshData LoadKhronos(const std::filesystem::path &path);
  MeshData LoadWavefront(const std::filesystem::path &path);

private:
  std::vector<Mesh> meshes_;
  std::vector<std::filesystem::path> images_;
  std::vector<Material> materials_;
  std::vector<SamplerSpecification> sampler_specifications_;
  std::vector<Meshlet> meshlets_;
  std::vector<Texture> textures_;

  uint32_t number_of_vertices_ = 0;
  uint32_t number_of_indices_ = 0;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_STORAGE_H
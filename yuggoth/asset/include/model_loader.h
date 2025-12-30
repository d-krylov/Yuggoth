#ifndef YUGGOTH_MODEL_LOADER_H
#define YUGGOTH_MODEL_LOADER_H

#include "mesh.h"
#include "material.h"
#include "yuggoth/graphics/core/graphics_specifications.h"
#include <filesystem>
#include <vector>

namespace Yuggoth {

class ModelLoader {
public:
  MeshData Load(const std::filesystem::path &path);

  std::span<const Mesh> GetMeshes() const;
  std::span<const Meshlet> GetMeshlets() const;
  std::span<const Texture> GetTextures() const;
  std::span<const Material> GetMaterials() const;
  std::span<const std::filesystem::path> GetImages() const;

  std::size_t GetNumberOfVertices();
  std::size_t GetNumberOfIndices();

protected:
  MeshData LoadKhronos(const std::filesystem::path &path);
  MeshData LoadWavefront(const std::filesystem::path &path);

  void BuildMeshlets();

private:
  std::vector<Meshlet> meshlets_;
  std::vector<Mesh> meshes_;
  std::vector<std::filesystem::path> images_;
  std::vector<Material> materials_;
  std::vector<SamplerSpecification> sampler_specifications_;
  std::vector<Texture> textures_;

  uint32_t number_of_vertices_ = 0;
  uint32_t number_of_indices_ = 0;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_LOADER_H
#include "yuggoth/asset/include/model_loader.h"
#include <print>

namespace Yuggoth {

void ModelLoader::Load(const std::filesystem::path &path) {
  auto extension = path.extension();
  if (extension == ".gltf") {
    LoadKhronos(path);
  }

  BuildMeshlets();
}

std::span<const Vertex> ModelLoader::GetVertices() const {
  return vertices_;
}

std::span<const uint32_t> ModelLoader::GetIndices() const {
  return indices_;
}

std::size_t ModelLoader::GetNumberOfVertices() {
  return vertices_.size();
}

std::size_t ModelLoader::GetNumberOfIndices() {
  return indices_.size();
}

std::span<const Meshlet> ModelLoader::GetMeshlets() const {
  return meshlets_;
}

std::span<const Mesh> ModelLoader::GetMeshes() const {
  return meshes_;
}

std::span<const std::filesystem::path> ModelLoader::GetTextures() const {
  return textures_;
}

void ModelLoader::BuildMeshlets() {
  uint32_t max_triangles = 21;
  uint32_t max_indices = max_triangles * 3;

  uint32_t offset = 0;
  uint32_t count = indices_.size();

  while (offset + 2 < count) {
    Meshlet meshlet;
    meshlet.index_offset_ = offset;

    uint32_t remaining = count - offset;
    uint32_t take = remaining < max_indices ? remaining : max_indices;

    take -= take % 3;

    meshlet.index_count_ = take;
    meshlets_.emplace_back(meshlet);

    offset += take;
  }
}

} // namespace Yuggoth
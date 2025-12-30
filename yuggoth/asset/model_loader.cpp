#include "yuggoth/asset/include/model_loader.h"
#include <print>

namespace Yuggoth {

MeshData ModelLoader::Load(const std::filesystem::path &path) {
  auto extension = path.extension();
  MeshData mesh_data;
  if (extension == ".gltf") mesh_data = LoadKhronos(path);

  return mesh_data;
}

std::span<const Meshlet> ModelLoader::GetMeshlets() const {
  return meshlets_;
}

std::span<const Mesh> ModelLoader::GetMeshes() const {
  return meshes_;
}

std::size_t ModelLoader::GetNumberOfVertices() {
  return number_of_vertices_;
}

std::size_t ModelLoader::GetNumberOfIndices() {
  return number_of_indices_;
}

std::span<const std::filesystem::path> ModelLoader::GetImages() const {
  return images_;
}

std::span<const Material> ModelLoader::GetMaterials() const {
  return materials_;
}

std::span<const Texture> ModelLoader::GetTextures() const {
  return textures_;
}

void ModelLoader::BuildMeshlets() {
}

} // namespace Yuggoth
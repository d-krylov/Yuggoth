#include "model_storage.h"
#include "meshoptimizer.h"
#include <print>

namespace Yuggoth {

MeshData ModelStorage::Load(const std::filesystem::path &path) {
  auto extension = path.extension();
  MeshData mesh_data;
  if (extension == ".gltf") mesh_data = LoadKhronos(path);

  return mesh_data;
}

std::span<const Meshlet> ModelStorage::GetMeshlets() const {
  return meshlets_;
}

std::span<const Mesh> ModelStorage::GetMeshes() const {
  return meshes_;
}

std::size_t ModelStorage::GetNumberOfVertices() const {
  return number_of_vertices_;
}

std::size_t ModelStorage::GetNumberOfIndices() const {
  return number_of_indices_;
}

std::span<const std::filesystem::path> ModelStorage::GetImages() const {
  return images_;
}

std::span<const Material> ModelStorage::GetMaterials() const {
  return materials_;
}

std::span<const Texture> ModelStorage::GetTextures() const {
  return textures_;
}

void ModelStorage::BuildMeshlets(std::span<const Vector3f> vertices, std::span<const uint32_t> indices) {
  auto max_meshlets = meshopt_buildMeshletsBound(indices.size(), MAX_VERTICES_IN_MESHLET, MAX_TRIANGLES_IN_MESHLET);

  std::vector<meshopt_Meshlet> meshlets(max_meshlets);
  std::vector<uint32_t> out_vertices(indices.size());
  std::vector<uint8_t> out_triangles(indices.size());

  auto positions = reinterpret_cast<const float *>(vertices.data());
  auto meshlet_count = meshopt_buildMeshlets(meshlets.data(), out_vertices.data(), out_triangles.data(), indices.data(), indices.size(), positions,
                                             vertices.size(), sizeof(Vector3f), MAX_TRIANGLES_IN_MESHLET, MAX_TRIANGLES_IN_MESHLET, 0);
}

} // namespace Yuggoth
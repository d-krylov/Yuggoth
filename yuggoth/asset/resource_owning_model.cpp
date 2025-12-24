#include "yuggoth/asset/include/resource_owning_model.h"
#include "yuggoth/graphics/acceleration/bottom_level_geometry.h"

namespace Yuggoth {

const Buffer &ResourceOwningModel::GetVertexBuffer() const {
  return vertex_buffer_;
}

const Buffer &ResourceOwningModel::GetIndexBuffer() const {
  return index_buffer_;
}

const Buffer &ResourceOwningModel::GetMeshletBuffer() const {
  return meshlet_buffer_;
}

const Buffer &ResourceOwningModel::GetMeshBuffer() const {
  return mesh_buffer_;
}

std::span<const Mesh> ResourceOwningModel::GetMeshes() const {
  return meshes_;
}

std::span<const Image2D> ResourceOwningModel::GetImages() const {
  return images_;
}

void ResourceOwningModel::SetVertices(std::span<const Vertex> vertices) {
  vertex_buffer_ = Buffer(vertices.size_bytes(), BufferUsageMaskBits::E_VERTEX_BUFFER_BIT | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, Buffer::CPU);
  vertex_buffer_.SetData(vertices);
}

void ResourceOwningModel::SetIndices(std::span<const uint32_t> indices) {
  index_buffer_ = Buffer(indices.size_bytes(), BufferUsageMaskBits::E_INDEX_BUFFER_BIT | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, Buffer::CPU);
  index_buffer_.SetData(indices);
}

void ResourceOwningModel::SetMeshlets(std::span<const Meshlet> meshlets) {
  meshlet_buffer_ = Buffer(meshlets.size_bytes(), BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, Buffer::CPU);
  meshlet_buffer_.SetData(meshlets);
}

void ResourceOwningModel::SetMeshes(std::span<const Mesh> meshes) {
  mesh_buffer_ = Buffer(meshes.size_bytes(), BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, Buffer::CPU);
  mesh_buffer_.SetData(meshes);
  meshes_ = std::vector(meshes.begin(), meshes.end());
}

void ResourceOwningModel::SetImages(std::span<const std::filesystem::path> pathes) {
  SamplerSpecification sampler_specification;
  sampler_specification.address_mode_ = SamplerAddressMode::E_REPEAT;
  for (const auto &path : pathes) {
    images_.emplace_back(path, sampler_specification);
  }
}

BottomLevelGeometry ResourceOwningModel::GetBottomLevelGeometry() const {
  BottomLevelGeometry bottom_geometry;
  auto vbo_size_bytes = vertex_buffer_.GetSize();
  auto ibo_size_bytes = index_buffer_.GetSize();
  BufferRange vbo_range(sizeof(Vertex), 0, vbo_size_bytes / sizeof(Vertex));
  BufferRange ibo_range(sizeof(uint32_t), 0, ibo_size_bytes / sizeof(uint32_t));
  GeometryMaskKHR mask = GeometryMaskBitsKHR::E_OPAQUE_BIT_KHR;
  bottom_geometry.AddTriangleGeometry(vertex_buffer_.GetBufferDeviceAddress(), vbo_range, index_buffer_.GetBufferDeviceAddress(), ibo_range, mask);
  return bottom_geometry;
}

AssetKind ResourceOwningModel::GetAssetKind() const {
  return AssetKind::RESOURCE_OWNING_MODEL;
}

} // namespace Yuggoth
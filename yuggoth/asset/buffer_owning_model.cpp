#include "yuggoth/asset/include/buffer_owning_model.h"

namespace Yuggoth {

const Buffer &BufferOwningModel::GetVertexBuffer() const {
  return vertex_buffer_;
}

const Buffer &BufferOwningModel::GetIndexBuffer() const {
  return index_buffer_;
}

void BufferOwningModel::SetVertices(std::span<const Vertex> vertices) {
  vertex_buffer_ = Buffer(vertices.size_bytes(), BufferUsageMaskBits::E_VERTEX_BUFFER_BIT, Buffer::CPU);
  vertex_buffer_.SetData(vertices);
}

void BufferOwningModel::SetIndices(std::span<const uint32_t> indices) {
  index_buffer_ = Buffer(indices.size_bytes(), BufferUsageMaskBits::E_INDEX_BUFFER_BIT, Buffer::CPU);
  index_buffer_.SetData(indices);
}

} // namespace Yuggoth
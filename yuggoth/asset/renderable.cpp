#include "yuggoth/asset/include/renderable.h"
#include "yuggoth/graphics/buffer/buffer.h"
#include "yuggoth/graphics/acceleration/bottom_level_geometry.h"

namespace Yuggoth {

BottomLevelGeometry Renderable::GetBottomLevelGeometry() const {
  BottomLevelGeometry bottom_level_geometry;
  auto vbo_buffer_range = GetVertexBufferRange();
  auto ibo_buffer_range = GetIndexBufferRange();
  auto vbo_address = Buffer::GetBufferDeviceAddress(vbo_buffer_range.buffer_);
  auto ibo_address = Buffer::GetBufferDeviceAddress(ibo_buffer_range.buffer_);
  ElementRange vbo_range(vbo_buffer_range.stride_, vbo_buffer_range.offset_, vbo_buffer_range.count_);
  ElementRange ibo_range(ibo_buffer_range.stride_, ibo_buffer_range.offset_, ibo_buffer_range.count_);
  bottom_level_geometry.AddTriangleGeometry(vbo_address, vbo_range, ibo_address, ibo_range, GeometryMaskBitsKHR::E_OPAQUE_BIT_KHR);
  return bottom_level_geometry;
}

BufferRange Renderable::GetVertexBufferRange() const {
  return BufferRange();
}

BufferRange Renderable::GetIndexBufferRange() const {
  return BufferRange();
}

BufferRange Renderable::GetMeshletBufferRange() const {
  return BufferRange();
}

const std::filesystem::path &Renderable::GetPath() const {
  return std::filesystem::path();
}

} // namespace Yuggoth
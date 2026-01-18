#include "resource_manager.h"
#include <yuggoth/graphics_device/core/graphics_device.h>
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/asset/model/mesh.h"
#include "yuggoth/graphics/core/graphics_context.h"
#include "yuggoth/scene/components/light.h"
#include "yuggoth/graphics/core/graphics_types.h"

namespace Yuggoth {

ResourceManager::ResourceManager(GraphicsDevice *graphics_device) : graphics_device_(graphics_device) {
}

std::size_t ResourceManager::UploadFromStagingBuffer(std::size_t type_id, std::span<const std::byte> data) {
  auto buffer_ci = BufferCreateInformation::CreateStagingBuffer(data.size_bytes());
  auto staging_buffer = graphics_device_->CreateBuffer(buffer_ci);
  auto &static_buffer = static_buffers_.at(type_id);
  staging_buffer->SetData(data);
  auto offset = static_buffer.allocator_.Allocate(data.size_bytes(), 0, Walle::VirtualAllocationCreateMaskBits::E_STRATEGY_MIN_OFFSET_BIT);
  CommandBuffer command_buffer(QueueType::TRANSFER);
  command_buffer.Begin(Walle::CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  BufferCopy buffer_copy(0, offset, data.size_bytes());
  command_buffer.CommandCopyBuffer(staging_buffer->GetHandle(), static_buffer.buffer_.GetHandle(), buffer_copy);
  command_buffer.CommandMemoryBarrier(PipelineStageMaskBits2::E_COPY_BIT, AccessMaskBits2::E_TRANSFER_WRITE_BIT, {}, {});
  command_buffer.End();
  command_buffer.Submit();
  return offset;
}

std::size_t ResourceManager::UploadStaticBuffer(std::size_t type_id, std::span<const std::byte> data) {
  std::size_t offset = 0;
  auto &static_buffer = static_buffers_.at(type_id);
  if (static_buffer.buffer_.IsAccessWithCPU()) {
    offset = static_buffer.allocator_.Allocate(data.size_bytes(), 0, Walle::VirtualAllocationCreateMaskBits::E_STRATEGY_MIN_OFFSET_BIT);
    static_buffer.buffer_.SetRawData(data, offset);
  } else {
    offset = UploadFromStagingBuffer(type_id, data);
  }
  return offset;
}

void ResourceManager::FreeRangeInStaticBuffer(const StaticBufferRange &static_buffer_range) {
  auto &static_buffer = static_buffers_.at(static_buffer_range.typeid_);
  auto offset_bytes = static_buffer_range.offset_ * static_buffer_range.stride_;
  static_buffer.allocator_.Free(offset_bytes);
}

} // namespace Yuggoth
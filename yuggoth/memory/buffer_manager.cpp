#include "yuggoth/memory/include/buffer_manager.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/asset/include/mesh.h"
#include "yuggoth/core/tools/include/core.h"
#include "yuggoth/scene/light/light.h"

namespace Yuggoth {

BufferManager::BufferManager() {
  Create();
}

void BufferManager::Create() {
  BufferUsageMask VERTEX_AND_STORAGE = BufferUsageMaskBits::E_VERTEX_BUFFER_BIT | BufferUsageMaskBits::E_STORAGE_BUFFER_BIT;
  AddBufferAllocator<Staging>(100_MiB, BufferUsageMaskBits::E_TRANSFER_SRC_BIT, CommonMasks::BUFFER_CPU);
  AddBufferAllocator<Index32>(100_MiB, BufferUsageMaskBits::E_INDEX_BUFFER_BIT | CommonMasks::BUFFER_USAGE_GPU, CommonMasks::BUFFER_CPU);
  AddBufferAllocator<Vertex>(100_MiB, VERTEX_AND_STORAGE | CommonMasks::BUFFER_USAGE_GPU, CommonMasks::BUFFER_CPU);
  AddBufferAllocator<IndexedIndirectCommand>(10_MiB, BufferUsageMaskBits::E_INDIRECT_BUFFER_BIT, CommonMasks::BUFFER_CPU);
  AddBufferAllocator<TransformMatrix>(10_MiB, BufferUsageMaskBits::E_STORAGE_BUFFER_BIT, CommonMasks::BUFFER_CPU);
}

BufferAllocator &BufferManager::GetBufferAllocator(std::size_t type_id) {
  return allocators_.at(type_id);
}

void BufferManager::UploadBuffer(CommandBuffer *command_buffer, const BufferRange &buffer_range, std::span<const std::byte> data,
                                 PipelineStageMask2 destination_stage, AccessMask2 destination_access) {
  auto &buffer_allocator = GetBufferAllocator(buffer_range.typeid_);
  if (buffer_allocator.buffer()->IsAccessWithCPU()) {
    auto destination_offset_byte = buffer_range.offset_ * buffer_range.stride_;
    buffer_allocator.buffer()->SetData(data, destination_offset_byte);
  } else {
    CopyBuffer(command_buffer, buffer_range, data, destination_stage, destination_access);
  }
}

void BufferManager::CopyBuffer(CommandBuffer *command_buffer, const BufferRange &buffer_range, std::span<const std::byte> data,
                               PipelineStageMask2 stage, AccessMask2 access) {
  auto &buffer_allocator = GetBufferAllocator(buffer_range.typeid_);
  auto &staging_allocator = GetBufferAllocator(Staging::type_id);
  auto staging_allocation = staging_allocator.allocate(data.size_bytes(), 0);
  auto source_offset_byte = staging_allocation.offset_ * staging_allocation.stride_;
  auto destination_offset_byte = buffer_range.offset_ * buffer_range.stride_;
  staging_allocator.buffer()->SetData(data, source_offset_byte);
  BufferCopy buffer_copy(source_offset_byte, destination_offset_byte, data.size_bytes());
  command_buffer->CommandCopyBuffer(staging_allocator.buffer()->GetHandle(), buffer_allocator.buffer()->GetHandle(), buffer_copy);
  command_buffer->CommandMemoryBarrier(PipelineStageMaskBits2::E_COPY_BIT, AccessMaskBits2::E_TRANSFER_WRITE_BIT, stage, access);
}

} // namespace Yuggoth
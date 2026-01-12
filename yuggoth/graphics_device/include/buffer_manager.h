#ifndef YUGGOTH_BUFFER_MANAGER_H
#define YUGGOTH_BUFFER_MANAGER_H

#include "buffer_allocator.h"

namespace Yuggoth {

class CommandBuffer;

class BufferManager {
public:
  BufferManager();

  BufferAllocator &GetBufferAllocator(std::size_t type_id);

  void UploadBuffer(CommandBuffer *command_buffer, const BufferRange &buffer_range, std::span<const std::byte> data,
                    Walle::PipelineStageMask2 destination_stage = {}, Walle::AccessMask2 destination_access = {});

protected:
  void Create();

  template <typename T> void AddBufferAllocator(const BufferCreateInformation &buffer_ci);

  void CopyBuffer(CommandBuffer *command_buffer, const BufferRange &buffer_range, std::span<const std::byte> data,
                  Walle::PipelineStageMask2 destination_stage, Walle::AccessMask2 destination_access);

private:
  std::unordered_map<std::size_t, BufferAllocator> allocators_;
};

} // namespace Yuggoth

#include "buffer_manager.ipp"

#endif // YUGGOTH_BUFFER_MANAGER_H
#ifndef YUGGOTH_BUFFER_CREATE_INFORMATION_H
#define YUGGOTH_BUFFER_CREATE_INFORMATION_H

#include "walle/walle.h"

namespace Yuggoth {

struct BufferCreateInformation {
  static BufferCreateInformation CreateStagingBuffer(uint64_t size);
  static BufferCreateInformation CreateCPUBuffer(uint64_t size, Walle::BufferUsageMask usage, bool mapped = false);
  static BufferCreateInformation CreateGPUBuffer(uint64_t size, Walle::BufferUsageMask usage);
  static BufferCreateInformation CreateAccelerationStructureBuffer(uint64_t size);
  static BufferCreateInformation CreateShaderBindingTableBuffer(uint64_t size);

  uint64_t buffer_size_{0};
  std::string buffer_name_;
  Walle::AllocationCreateMask allocator_mask_;
  Walle::BufferUsageMask buffer_usage_;
  Walle::MemoryPropertyMask memory_property_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_CREATE_INFORMATION_H
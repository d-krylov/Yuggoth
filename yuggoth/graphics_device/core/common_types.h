#ifndef YUGGOTH_COMMON_TYPES_H
#define YUGGOTH_COMMON_TYPES_H

#include <vulkan/vulkan.h>
#include "yuggoth/scene/components/light.h"
#include "yuggoth/asset/model/mesh.h"

namespace Yuggoth {

struct BufferRange {
  VkBuffer buffer_ = VK_NULL_HANDLE;
  uint64_t typeid_ = 0;
  uint32_t stride_ = 0; // element size
  uint32_t offset_ = 0; // base element index
  uint32_t count_ = 0;  // number of elements
};

struct Staging {
  static inline const std::size_t type_id = GetTypeId<Staging>();

  uint8_t data_;
};

struct Index32 {
  static inline const std::size_t type_id = GetTypeId<Index32>();

  uint32_t data_;
};

struct IndexedIndirectCommand {
  static inline const std::size_t type_id = GetTypeId<IndexedIndirectCommand>();

  uint32_t index_count_ = 0;
  uint32_t instance_count_ = 0;
  uint32_t first_index_ = 0;
  int32_t vertex_offset_ = 0;
  uint32_t first_instance_ = 0;
};

struct TransformMatrix {
  static inline const std::size_t type_id = GetTypeId<TransformMatrix>();

  Matrix4f transform_;
};

} // namespace Yuggoth

#endif // YUGGOTH_COMMON_TYPES_H
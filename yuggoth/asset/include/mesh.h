#ifndef YUGGOTH_MESH_H
#define YUGGOTH_MESH_H

#include "yuggoth/mathematics/include/mathematics_types.h"

namespace Yuggoth {

struct Vertex {
  Vector3f position_;
  Vector3f normal_;
  Vector2f uv_;
};

struct BufferInformation {
  uint32_t buffer_stride_ = 0; // size of element in buffer
  uint32_t buffer_offset_ = 0; // offset in elements
  uint32_t buffer_count_ = 0;  // size in elements
  uint64_t buffer_uuid_ = 0;
};

struct Mesh {};

} // namespace Yuggoth

#endif // YUGGOTH_MESH_H
#ifndef YUGGOTH_MESH_H
#define YUGGOTH_MESH_H

#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/core/tools/include/core.h"

namespace Yuggoth {

struct Vertex {
  static inline const std::size_t type_id = GetTypeId<Vertex>();

  Vector3f position_;
  Vector3f normal_;
  Vector2f uv_;
};

struct Mesh {
  static inline const std::size_t type_id = GetTypeId<Mesh>();

  int32_t color_texture_index = 0;
  int32_t normal_texture_index = 0;
  int32_t vertices_offset = 0;
  int32_t indices_offset = 0;
  int32_t indices_size = 0;
};

struct Meshlet {
  uint32_t index_offset_ = 0;
  uint32_t index_count_ = 0;
};

} // namespace Yuggoth

#endif // YUGGOTH_MESH_H
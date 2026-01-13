#ifndef YUGGOTH_MESH_H
#define YUGGOTH_MESH_H

#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/core/tools/core.h"
#include <vector>

namespace Yuggoth {

struct Vertex {
  static inline const std::size_t type_id = GetTypeId<Vertex>();

  Vector3f position_;
  Vector3f normal_;
  Vector2f uv_;
};

struct Mesh {
  static inline const std::size_t type_id = GetTypeId<Mesh>();

  int32_t material_index_ = -1;
  int32_t vertices_offset_ = -1;
  int32_t indices_offset_ = -1;
  int32_t indices_count_ = -1;
};

struct Meshlet {
  int32_t index_offset_ = -1;
  int32_t index_count_ = -1;
};

struct Texture {
  int32_t image_index_ = -1;
  int32_t sampler_index_ = -1;
};

struct MeshData {
  // clang-format off
  std::span<const Vertex> GetVertices() const { return vertices_; }
  std::span<const uint32_t> GetIndices() const { return indices_; }
  // clang-format on
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
};

inline constexpr auto MAX_VERTICES_IN_MESHLET = 64;
inline constexpr auto MAX_TRIANGLES_IN_MESHLET = 96;

} // namespace Yuggoth

#endif // YUGGOTH_MESH_H
#ifndef YUGGOTH_MESH_H
#define YUGGOTH_MESH_H

#include "yuggoth/mathematics/include/mathematics_types.h"
#include "yuggoth/memory/include/buffer_manager.h"

namespace Yuggoth {

struct Vertex {
  Vector3f position_;
  Vector3f normal_;
  Vector2f uv_;
};

static_assert(std::popcount(sizeof(Vertex)) == 1, "Size of Vertex should be power of 2");

struct Mesh {};

} // namespace Yuggoth

#endif // YUGGOTH_MESH_H
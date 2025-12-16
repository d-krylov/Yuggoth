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

struct Mesh {};

} // namespace Yuggoth

#endif // YUGGOTH_MESH_H
#ifndef YUGGOTH_PRIMITIVES_H
#define YUGGOTH_PRIMITIVES_H

#include "mesh.h"

namespace Yuggoth {

enum class PrimitiveKind {
  HEXAHEDRON,
  OCTAHEDRON
};

void SetNormals(std::span<Vertex> vertices, std::span<const uint32_t> indices);

MeshData GetPrimitive(PrimitiveKind primitive_kind);

} // namespace Yuggoth

#endif // YUGGOTH_PRIMITIVES_H
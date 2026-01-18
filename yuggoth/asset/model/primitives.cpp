#include "primitives.h"
#include <utility>

namespace Yuggoth {

void SetNormals(std::span<Vertex> vertices, std::span<const uint32_t> indices) {
  for (auto i = 0; i < indices.size(); i += 3) {
    auto p0 = vertices[indices[i + 0]].position_;
    auto p1 = vertices[indices[i + 1]].position_;
    auto p2 = vertices[indices[i + 2]].position_;
    auto nn = glm::normalize(glm::cross(p1 - p0, p2 - p0));
    vertices[indices[i + 0]].normal_ = nn;
    vertices[indices[i + 1]].normal_ = nn;
    vertices[indices[i + 2]].normal_ = nn;
  }
}

MeshData GetOctahedron() {
  auto a = 0.5f / std::sqrt(2.0f);
  auto b = 0.5f;

  std::vector<Vertex> vertices{
    {{-a, +0, +a}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // LEFT NEAR
    {{+a, +0, +a}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // RIGHT NEAR
    {{+a, +0, -a}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // RIGHT FAR
    {{-a, +0, -a}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // LEFT FAR
    {{+0, +b, +0}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // UP
    {{+0, -b, +0}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, // DOWN
  };

  std::vector<uint32_t> indices{
    0, 1, 4, // UP FRONT
    1, 2, 4, // UP RIGHT
    2, 3, 4, // UP BACK
    3, 0, 4, // UP LEFT
    0, 5, 1, // DOWN FRONT
    1, 5, 2, // DOWN RIGHT
    2, 5, 3, // DOWN BACK
    3, 5, 0  // DOWN LEFT
  };

  return MeshData(vertices, indices);
}

MeshData GetHexahedron() {
  std::vector<Vertex> vertices{
    {{-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{+0.5f, -0.5f, +0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{+0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{-0.5f, +0.5f, +0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{+0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}, //
    {{-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}}  //
  };

  std::vector<uint32_t> indices{
    0, 1, 2, 2, 3, 0, // FRONT
    1, 5, 6, 6, 2, 1, // RIGHT
    5, 4, 7, 7, 6, 5, // BACK
    4, 0, 3, 3, 7, 4, // LEFT
    3, 2, 6, 6, 7, 3, // UP
    4, 5, 1, 1, 0, 4  // DOWN
  };

  return MeshData(vertices, indices);
}

MeshData GetPrimitive(PrimitiveKind primitive_kind) {
  switch (primitive_kind) {
  case PrimitiveKind::HEXAHEDRON: return GetHexahedron();
  case PrimitiveKind::OCTAHEDRON: return GetOctahedron();
  default: std::unreachable(); break;
  }
}

} // namespace Yuggoth
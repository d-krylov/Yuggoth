#ifndef YUGGOTH_MODEL_BASE_H
#define YUGGOTH_MODEL_BASE_H

#include "yuggoth/memory/include/common_types.h"
#include "asset.h"
#include "mesh.h"
#include <filesystem>

namespace Yuggoth {

class BottomLevelGeometry;

class Renderable : public Asset {
public:
  virtual ~Renderable() = default;

  virtual BufferRange GetVertexBufferRange() const;
  virtual BufferRange GetIndexBufferRange() const;
  virtual BufferRange GetMeshletBufferRange() const;

  virtual const std::filesystem::path &GetPath() const;

  BottomLevelGeometry GetBottomLevelGeometry() const;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_BASE_H
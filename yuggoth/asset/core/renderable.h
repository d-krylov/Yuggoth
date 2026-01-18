#ifndef YUGGOTH_MODEL_BASE_H
#define YUGGOTH_MODEL_BASE_H

#include "yuggoth/graphics_device/systems/resource_manager_types.h"
#include "yuggoth/asset/model/mesh.h"
#include "asset.h"
#include <filesystem>

namespace Yuggoth {

class BottomLevelGeometry;

class Renderable : public Asset {
public:
  virtual ~Renderable() = default;

  virtual StaticBufferRange GetVertexBufferRange() const;
  virtual StaticBufferRange GetIndexBufferRange() const;
  virtual StaticBufferRange GetMeshletBufferRange() const;

  virtual const std::filesystem::path &GetPath() const;

  BottomLevelGeometry GetBottomLevelGeometry() const;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_BASE_H
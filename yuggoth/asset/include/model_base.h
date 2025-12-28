#ifndef YUGGOTH_MODEL_BASE_H
#define YUGGOTH_MODEL_BASE_H

#include "yuggoth/memory/include/common_types.h"
#include "asset.h"
#include "mesh.h"

namespace Yuggoth {

class ModelBase : public Asset {
public:
  virtual ~ModelBase() = default;

  virtual BufferRange GetVertexBufferRange() const {
    return BufferRange();
  }

  virtual BufferRange GetIndexBufferRange() const {
    return BufferRange();
  }

  virtual BufferRange GetMeshBufferRange() const {
    return BufferRange();
  }

  virtual BufferRange GetMeshletBufferRange() const {
    return BufferRange();
  }
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_BASE_H
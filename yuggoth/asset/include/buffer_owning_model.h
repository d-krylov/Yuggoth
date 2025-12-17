#ifndef YUGGOTH_BUFFER_OWNING_MODEL_H
#define YUGGOTH_BUFFER_OWNING_MODEL_H

#include "asset.h"
#include "mesh.h"

namespace Yuggoth {

class BufferOwningModel : public Asset {
public:
  const Buffer &GetVertexBuffer() const;
  const Buffer &GetIndexBuffer() const;

  void SetVertices(std::span<const Vertex> vertices);
  void SetIndices(std::span<const uint32_t> indices);

private:
  Buffer vertex_buffer_;
  Buffer index_buffer_;
};

} // namespace Yuggoth

#endif // YUGGOTH_BUFFER_OWNING_MODEL_H

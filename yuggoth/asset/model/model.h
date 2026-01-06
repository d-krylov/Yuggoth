#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "yuggoth/asset/core/renderable.h"

namespace Yuggoth {

class Model : public Renderable {
public:
  Model(const BufferRange &vertices_range, const BufferRange &indices_range, const std::filesystem::path &path = "");

  BufferRange GetVertexBufferRange() const override;
  BufferRange GetIndexBufferRange() const override;

  const std::filesystem::path &GetPath() const override;

  AssetKind GetAssetKind() const override;

private:
  std::filesystem::path path_;
  BufferRange vertices_range_;
  BufferRange indices_range_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
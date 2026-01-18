#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "yuggoth/asset/core/renderable.h"

namespace Yuggoth {

class Model : public Renderable {
public:
  Model(const StaticBufferRange &vertices_range, const StaticBufferRange &indices_range, const std::filesystem::path &path = "");

  StaticBufferRange GetVertexBufferRange() const override;
  StaticBufferRange GetIndexBufferRange() const override;

  const std::filesystem::path &GetPath() const override;

  AssetKind GetAssetKind() const override;

private:
  std::filesystem::path path_;
  StaticBufferRange vertices_range_;
  StaticBufferRange indices_range_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
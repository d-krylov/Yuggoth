#include "yuggoth/asset/include/model.h"

namespace Yuggoth {

BufferRange Model::GetVertexBufferRange() const {
  return vertices_range_;
}

BufferRange Model::GetIndexBufferRange() const {
  return indices_range_;
}

const std::filesystem::path &Model::GetPath() const {
  return path_;
}

Model::Model(const BufferRange &vertices_range, const BufferRange &indices_range, const std::filesystem::path &path)
  : vertices_range_(vertices_range), indices_range_(indices_range), path_(path) {
}

AssetKind Model::GetAssetKind() const {
  return AssetKind::MODEL;
}

} // namespace Yuggoth
#include "yuggoth/asset/include/model.h"
#include "yuggoth/graphics/core/graphics_structures.h"
#include "yuggoth/graphics/acceleration/bottom_level_geometry.h"

namespace Yuggoth {

Model::Model(const std::filesystem::path &model_path) : model_path_(model_path) {
}

const std::filesystem::path &Model::GetModelPath() const {
  return model_path_;
}

BufferRange Model::GetVertexBufferRange() const {
  return vertices_;
}

BufferRange Model::GetIndexBufferRange() const {
  return indices_;
}

void Model::SetVertexBufferRange(const BufferRange &vertices) {
  vertices_ = vertices;
}

void Model::SetIndexBufferRange(const BufferRange &indices) {
  indices_ = indices;
}

AssetKind Model::GetAssetKind() const {
  return AssetKind::MODEL;
}

BottomLevelGeometry Model::GetBottomLevelGeometry() const {
}

} // namespace Yuggoth
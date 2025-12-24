#include "yuggoth/asset/include/model.h"
#include "yuggoth/graphics/core/graphics_structures.h"
#include "yuggoth/graphics/acceleration/bottom_level_geometry.h"

namespace Yuggoth {

Model::Model(const std::filesystem::path &model_path) : model_path_(model_path) {
}

const std::filesystem::path &Model::GetModelPath() const {
  return model_path_;
}

const BufferRange &Model::GetVerticesInformation() const {
  return vertices_;
}

const BufferRange &Model::GetIndicesInformation() const {
  return indices_;
}

void Model::SetVerticesInformation(const BufferRange &vertices) {
  vertices_ = vertices;
}

void Model::SetIndicesInformation(const BufferRange &indices) {
  indices_ = indices;
}

AssetKind Model::GetAssetKind() const {
  return AssetKind::MODEL;
}

BottomLevelGeometry Model::GetBottomLevelGeometry() const {
}

} // namespace Yuggoth
#include "yuggoth/asset/include/model.h"
#include "yuggoth/graphics/core/graphics_structures.h"

namespace Yuggoth {

Model::Model(const std::filesystem::path &model_path) : model_path_(model_path) {
}

const std::filesystem::path &Model::GetModelPath() const {
  return model_path_;
}

const BufferRangeInformation &Model::GetVerticesInformation() const {
  return vertices_;
}

const BufferRangeInformation &Model::GetIndicesInformation() const {
  return indices_;
}

void Model::SetVerticesInformation(const BufferRangeInformation &vertices) {
  vertices_ = vertices;
}

void Model::SetIndicesInformation(const BufferRangeInformation &indices) {
  indices_ = indices;
}

AssetKind Model::GetAssetKind() const {
  return AssetKind::MODEL;
}

} // namespace Yuggoth
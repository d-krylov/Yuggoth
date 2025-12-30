#include "yuggoth/asset/include/asset_storage.h"

namespace Yuggoth {

ModelLoader &AssetStorage::AddModelLoader(const std::filesystem::path &path) {
  return model_loaders_[path];
}

const ModelLoader &AssetStorage::GetModelLoader(const std::filesystem::path &path) const {
  return model_loaders_.at(path);
}

} // namespace Yuggoth

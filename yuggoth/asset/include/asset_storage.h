#ifndef YUGGOTH_ASSET_STORAGE_H
#define YUGGOTH_ASSET_STORAGE_H

#include "model_loader.h"
#include <unordered_map>
#include <memory>

namespace Yuggoth {

class AssetStorage {
public:
  ModelLoader &AddModelLoader(const std::filesystem::path &path);

  const ModelLoader &GetModelLoader(const std::filesystem::path &path) const;

private:
  std::unordered_map<std::filesystem::path, ModelLoader> model_loaders_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_STORAGE_H
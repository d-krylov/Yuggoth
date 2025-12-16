#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/asset/include/model_loader.h"
#include "yuggoth/asset/include/model.h"
#include "yuggoth/memory/include/buffer_manager.h"

namespace Yuggoth {

AssetManager::AssetManager(BufferManager *buffer_manager) : buffer_manager_(buffer_manager) {
}

void AssetManager::RegisterModel(const std::filesystem::path &path) {
  ModelLoader model_loader;
  model_loader.Load(path);
}

} // namespace Yuggoth
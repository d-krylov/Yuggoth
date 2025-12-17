#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/asset/include/model_loader.h"
#include "yuggoth/asset/include/model.h"
#include "yuggoth/asset/include/buffer_owning_model.h"
#include "yuggoth/memory/include/buffer_manager.h"

namespace Yuggoth {

AssetManager::AssetManager(BufferManager *buffer_manager) : buffer_manager_(buffer_manager) {
}

std::shared_ptr<BufferOwningModel> AssetManager::RegisterModel(const std::filesystem::path &path) {
  ModelLoader model_loader;
  model_loader.Load(path);
  auto model = std::make_shared<BufferOwningModel>();
  model->SetVertices(model_loader.GetVertices());
  model->SetIndices(model_loader.GetIndices());
  assets_[model->uuid_] = model;
  return model;
}

} // namespace Yuggoth
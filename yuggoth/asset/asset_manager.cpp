#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/asset/include/model_loader.h"
#include "yuggoth/asset/include/model.h"
#include "yuggoth/asset/include/buffer_owning_model.h"
#include "yuggoth/memory/include/buffer_manager.h"

namespace Yuggoth {

AssetManager::AssetManager(BufferManager *buffer_manager) : buffer_manager_(buffer_manager) {
}

std::shared_ptr<BufferOwningModel> AssetManager::RegisterBufferOwningModel(const std::filesystem::path &path) {
  if (uuids_.contains(path)) {
    auto uuid = uuids_[path];
    std::shared_ptr<Asset> asset = assets_[uuid].lock();
    return std::static_pointer_cast<BufferOwningModel>(asset);
  }
  ModelLoader model_loader;
  model_loader.Load(path);
  auto model = std::make_shared<BufferOwningModel>();
  model->SetVertices(model_loader.GetVertices());
  model->SetIndices(model_loader.GetIndices());
  assets_[model->uuid_] = model;
  uuids_[path] = model->uuid_;
  return model;
}

void AssetManager::Update() {
  for (auto it = uuids_.begin(); it != uuids_.end();) {
    auto asset_it = assets_.find(it->second);
    if (asset_it == assets_.end() || asset_it->second.expired()) {
      assets_.erase(it->second);
      it = uuids_.erase(it);
    } else {
      ++it;
    }
  }
}

std::shared_ptr<Model> AssetManager::RegisterModel(const std::filesystem::path &path) {
  if (uuids_.contains(path)) {
    auto uuid = uuids_[path];
    std::shared_ptr<Asset> asset = assets_[uuid].lock();
  }
  ModelLoader model_loader;
  model_loader.Load(path);

  auto &vertex_allocator = buffer_manager_->GetVertexAllocator();
  auto &index_allocator = buffer_manager_->GetIndexAllocator();

  auto vertex_information = vertex_allocator.Allocate(model_loader.GetNumberOfVertices(), 0);
  auto index_information = index_allocator.Allocate(model_loader.GetNumberOfIndices(), 0);

  vertex_allocator.GetBuffer()->SetData(model_loader.GetVertices(), vertex_information.offset_ * vertex_information.stride_);
  index_allocator.GetBuffer()->SetData(model_loader.GetIndices(), index_information.offset_ * index_information.stride_);

  auto model = std::make_shared<Model>(path);

  model->SetVerticesInformation(vertex_information);
  model->SetIndicesInformation(index_information);

  assets_[model->uuid_] = model;
  uuids_[path] = model->uuid_;

  return model;
}

} // namespace Yuggoth
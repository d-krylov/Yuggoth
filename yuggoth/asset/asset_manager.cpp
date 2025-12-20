#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/asset/include/model_loader.h"
#include "yuggoth/asset/include/model.h"
#include "yuggoth/asset/include/buffer_owning_model.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include <print>

namespace Yuggoth {

AssetManager::AssetManager(BufferManager *buffer_manager) : buffer_manager_(buffer_manager) {
}

IntrusivePointer<BufferOwningModel> AssetManager::RegisterBufferOwningModel(const std::filesystem::path &path) {
  if (uuids_.contains(path)) {
    auto uuid = uuids_[path];
    return IntrusivePointer<BufferOwningModel>(static_cast<BufferOwningModel *>(assets_[uuid].get()));
  }
  ModelLoader model_loader;
  model_loader.Load(path);
  auto model = MakeIntrusivePointer<BufferOwningModel>();
  model->SetVertices(model_loader.GetVertices());
  model->SetIndices(model_loader.GetIndices());
  assets_[model->uuid_] = model;
  uuids_[path] = model->uuid_;
  return model;
}

void AssetManager::Update() {
  for (auto it = uuids_.begin(); it != uuids_.end();) {
    auto asset_it = assets_.find(it->second);
    if (asset_it != assets_.end() && asset_it->second.use_count() == 1) {
      std::println("Try delete");
      auto *model = static_cast<Model *>(asset_it->second.get());
      auto &vertices = model->GetVerticesInformation();
      auto &indices = model->GetIndicesInformation();
      buffer_manager_->GetVertexAllocator().Free(vertices.offset_ * vertices.stride_);
      buffer_manager_->GetIndexAllocator().Free(indices.offset_ * indices.stride_);
      assets_.erase(asset_it);
      it = uuids_.erase(it);
    } else {
      ++it;
    }
  }
}

IntrusivePointer<Model> AssetManager::RegisterModel(const std::filesystem::path &path) {
  if (uuids_.contains(path)) {
    auto uuid = uuids_[path];
    return IntrusivePointer<Model>(static_cast<Model *>(assets_[uuid].get()));
  }
  ModelLoader model_loader;
  model_loader.Load(path);

  auto &vertex_allocator = buffer_manager_->GetVertexAllocator();
  auto &index_allocator = buffer_manager_->GetIndexAllocator();

  auto vertex_information = vertex_allocator.Allocate(model_loader.GetNumberOfVertices(), 0);
  auto index_information = index_allocator.Allocate(model_loader.GetNumberOfIndices(), 0);

  vertex_allocator.GetBuffer()->SetData(model_loader.GetVertices(), vertex_information.offset_ * vertex_information.stride_);
  index_allocator.GetBuffer()->SetData(model_loader.GetIndices(), index_information.offset_ * index_information.stride_);

  auto model = MakeIntrusivePointer<Model>(path);

  model->SetVerticesInformation(vertex_information);
  model->SetIndicesInformation(index_information);

  assets_[model->uuid_] = model;
  uuids_[path] = model->uuid_;

  return model;
}

} // namespace Yuggoth
#include "yuggoth/asset/include/asset_manager.h"
#include "yuggoth/asset/include/model_loader.h"
#include "yuggoth/asset/include/model.h"
#include "yuggoth/asset/include/resource_owning_model.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include "yuggoth/core/tools/include/image_wrapper.h"
#include "yuggoth/graphics/command/command_buffer.h"

#include <print>

namespace Yuggoth {

AssetManager::AssetManager(BufferManager *buffer_manager) : buffer_manager_(buffer_manager) {
}

IntrusivePointer<ResourceOwningModel> AssetManager::RegisterResourceOwningModel(const std::filesystem::path &path) {
  if (uuids_.contains(path)) {
    auto uuid = uuids_[path];
    return IntrusivePointer<ResourceOwningModel>(static_cast<ResourceOwningModel *>(assets_[uuid].get()));
  }
  ModelLoader model_loader;
  model_loader.Load(path);
  auto model = MakeIntrusivePointer<ResourceOwningModel>();
  model->SetVertices(model_loader.GetVertices());
  model->SetIndices(model_loader.GetIndices());
  model->SetMeshlets(model_loader.GetMeshlets());
  model->SetMeshes(model_loader.GetMeshes());
  model->SetImages(model_loader.GetTextures());
  assets_[model->uuid_] = model;
  uuids_[path] = model->uuid_;
  return model;
}

void AssetManager::Update() {
  for (auto it = uuids_.begin(); it != uuids_.end();) {
    auto asset_it = assets_.find(it->second);
    if (asset_it != assets_.end() && asset_it->second.use_count() == 2) {
      if (asset_it->second->GetAssetKind() == AssetKind::MODEL) {
        auto *model = static_cast<ModelBase *>(asset_it->second.get());
        auto vertices = model->GetVertexBufferRange();
        auto indices = model->GetIndexBufferRange();
        buffer_manager_->GetBufferAllocator(Vertex::type_id).free(vertices.offset_ * vertices.stride_);
        buffer_manager_->GetBufferAllocator(Index32::type_id).free(indices.offset_ * indices.stride_);
      }

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

  auto &vertex_allocator = buffer_manager_->GetBufferAllocator(Vertex::type_id);
  auto &index_allocator = buffer_manager_->GetBufferAllocator(Index32::type_id);

  auto vertices_range = vertex_allocator.allocate(model_loader.GetNumberOfVertices(), 0);
  auto indices_range = index_allocator.allocate(model_loader.GetNumberOfIndices(), 0);

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  buffer_manager_->UploadBuffer(&command_buffer, vertices_range, std::as_bytes(model_loader.GetVertices()));
  buffer_manager_->UploadBuffer(&command_buffer, indices_range, std::as_bytes(model_loader.GetIndices()));
  command_buffer.End();
  command_buffer.Submit();

  auto model = MakeIntrusivePointer<Model>(path);

  model->SetVertexBufferRange(vertices_range);
  model->SetIndexBufferRange(indices_range);

  assets_[model->uuid_] = model;
  uuids_[path] = model->uuid_;

  return model;
}

IntrusivePointer<Image2D> AssetManager::RegisterTexture(const std::filesystem::path &path) {
  if (uuids_.contains(path)) {
    auto uuid = uuids_[path];
    return IntrusivePointer<Image2D>(static_cast<Image2D *>(assets_[uuid].get()));
  }

  ImageWrapper image(path);
  auto texture = MakeIntrusivePointer<Image2D>(image.GetWidth(), image.GetHeight(), image.GetData(), SamplerSpecification());
}

} // namespace Yuggoth
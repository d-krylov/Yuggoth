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

void AssetManager::Update() {
  for (auto it = outer_uuids_.begin(); it != outer_uuids_.end();) {
    auto asset_it = assets_.find(it->second);
    if (asset_it != assets_.end() && asset_it->second.use_count() == 2) {
      if (asset_it->second->GetAssetKind() == AssetKind::MODEL) {
        auto *model = static_cast<Renderable *>(asset_it->second.get());
        auto vertices = model->GetVertexBufferRange();
        auto indices = model->GetIndexBufferRange();
        buffer_manager_->GetBufferAllocator(Vertex::type_id).free(vertices.offset_ * vertices.stride_);
        buffer_manager_->GetBufferAllocator(Index32::type_id).free(indices.offset_ * indices.stride_);
      }

      assets_.erase(asset_it);
      it = outer_uuids_.erase(it);
    } else {
      ++it;
    }
  }
}

IntrusivePointer<Model> AssetManager::RegisterModel(PrimitiveKind primitive_kind) {
  if (primitives_uuids_.contains(primitive_kind)) {
    auto uuid = primitives_uuids_[primitive_kind];
    return IntrusivePointer<Model>(static_cast<Model *>(assets_[uuid].get()));
  }

  auto &vertex_allocator = buffer_manager_->GetBufferAllocator(Vertex::type_id);
  auto &index_allocator = buffer_manager_->GetBufferAllocator(Index32::type_id);

  auto primitive = GetPrimitive(primitive_kind);
  SetNormals(primitive.vertices_, primitive.indices_);

  auto vertices_range = vertex_allocator.allocate(primitive.vertices_.size(), 0);
  auto indices_range = index_allocator.allocate(primitive.indices_.size(), 0);

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  buffer_manager_->UploadBuffer(&command_buffer, vertices_range, std::as_bytes(std::span(primitive.vertices_)));
  buffer_manager_->UploadBuffer(&command_buffer, indices_range, std::as_bytes(std::span(primitive.indices_)));
  command_buffer.End();
  command_buffer.Submit();

  auto model = MakeIntrusivePointer<Model>(vertices_range, indices_range);

  assets_[model->uuid_] = model;
  primitives_uuids_[primitive_kind] = model->uuid_;

  return model;
}

IntrusivePointer<Model> AssetManager::RegisterModel(const std::filesystem::path &path) {
  if (outer_uuids_.contains(path)) {
    auto uuid = outer_uuids_[path];
    return IntrusivePointer<Model>(static_cast<Model *>(assets_[uuid].get()));
  }
  auto &model_loader = asset_storage_.AddModelLoader(path);
  auto mesh_data = model_loader.Load(path);

  auto &vertex_allocator = buffer_manager_->GetBufferAllocator(Vertex::type_id);
  auto &index_allocator = buffer_manager_->GetBufferAllocator(Index32::type_id);

  auto vertices_range = vertex_allocator.allocate(model_loader.GetNumberOfVertices(), 0);
  auto indices_range = index_allocator.allocate(model_loader.GetNumberOfIndices(), 0);

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  buffer_manager_->UploadBuffer(&command_buffer, vertices_range, std::as_bytes(mesh_data.GetVertices()));
  buffer_manager_->UploadBuffer(&command_buffer, indices_range, std::as_bytes(mesh_data.GetIndices()));
  command_buffer.End();
  command_buffer.Submit();

  auto model = MakeIntrusivePointer<Model>(vertices_range, indices_range, path);

  assets_[model->uuid_] = model;
  outer_uuids_[path] = model->uuid_;

  return model;
}

IntrusivePointer<Image2D> AssetManager::RegisterTexture(const std::filesystem::path &path) {
  if (outer_uuids_.contains(path)) {
    auto uuid = outer_uuids_[path];
    return IntrusivePointer<Image2D>(static_cast<Image2D *>(assets_[uuid].get()));
  }

  ImageWrapper image(path);
  auto texture = MakeIntrusivePointer<Image2D>(image.GetWidth(), image.GetHeight(), image.GetData(), SamplerSpecification());
}

const AssetStorage &AssetManager::GetAssetStorage() const {
  return asset_storage_;
}

} // namespace Yuggoth
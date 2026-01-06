#include "asset_manager.h"
#include "yuggoth/asset/model/model_storage.h"
#include "yuggoth/asset/model/model.h"
#include "yuggoth/memory/include/buffer_manager.h"
#include "yuggoth/core/tools/include/image_wrapper.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include <print>

namespace Yuggoth {

AssetManager::AssetManager(const AssetManagerCreateContext &create_context) : create_context_(create_context) {
}

void AssetManager::Update() {
  for (auto it = outer_uuids_.begin(); it != outer_uuids_.end();) {
    auto asset_it = assets_.find(it->second);
    if (asset_it != assets_.end() && asset_it->second.use_count() == 1) {
      if (asset_it->second->GetAssetKind() == AssetKind::MODEL) {
        auto *model = static_cast<Renderable *>(asset_it->second.get());
        auto vertices = model->GetVertexBufferRange();
        auto indices = model->GetIndexBufferRange();
        create_context_.buffer_manager_->GetBufferAllocator(Vertex::type_id).free(vertices.offset_ * vertices.stride_);
        create_context_.buffer_manager_->GetBufferAllocator(Index32::type_id).free(indices.offset_ * indices.stride_);
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
    return static_pointer_cast<Model>(assets_.at(uuid));
  }

  auto primitive = GetPrimitive(primitive_kind);
  SetNormals(primitive.vertices_, primitive.indices_);

  auto mesh_buffer_range = PutMeshDataInBuffers(primitive);

  auto model = MakeIntrusivePointer<Model>(mesh_buffer_range.first, mesh_buffer_range.second);

  assets_[model->uuid_] = model;
  primitives_uuids_[primitive_kind] = model->uuid_;

  return model;
}

IntrusivePointer<Model> AssetManager::RegisterModel(const std::filesystem::path &path) {
  if (outer_uuids_.contains(path)) {
    auto uuid = outer_uuids_[path];
    return static_pointer_cast<Model>(assets_.at(uuid));
  }

  ModelStorage model_storage;
  auto mesh_data = model_storage.Load(path);
  auto mesh_buffer_range = PutMeshDataInBuffers(mesh_data);
  auto model = MakeIntrusivePointer<Model>(mesh_buffer_range.first, mesh_buffer_range.second, path);
  asset_storage_.AddModelStorage(model->uuid_, std::move(model_storage));

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

MeshBufferRange AssetManager::PutMeshDataInBuffers(const MeshData &mesh_data) {
  auto buffer_manager = create_context_.buffer_manager_;

  auto &vertex_allocator = buffer_manager->GetBufferAllocator(Vertex::type_id);
  auto &index_allocator = buffer_manager->GetBufferAllocator(Index32::type_id);

  auto vertices_range = vertex_allocator.allocate(mesh_data.vertices_.size(), 0);
  auto indices_range = index_allocator.allocate(mesh_data.indices_.size(), 0);

  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);

  buffer_manager->UploadBuffer(&command_buffer, vertices_range, std::as_bytes(mesh_data.GetVertices()));
  buffer_manager->UploadBuffer(&command_buffer, indices_range, std::as_bytes(mesh_data.GetIndices()));

  command_buffer.End();
  command_buffer.Submit();

  return MeshBufferRange(vertices_range, indices_range);
}

} // namespace Yuggoth
#include "asset_manager.h"
#include "yuggoth/asset/model/model_storage.h"
#include "yuggoth/asset/model/model.h"
#include "yuggoth/graphics_device/systems/buffer_manager.h"
#include "yuggoth/core/tools/image_wrapper.h"
#include "yuggoth/graphics/command/command_buffer.h"
#include "yuggoth/graphics_device/core/graphics_device.h"
#include "yuggoth/graphics/core/graphics_context.h"
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

  RegisterModelImages(model_storage);
  asset_storage_.AddModelStorage(model->uuid_, std::move(model_storage));

  assets_[model->uuid_] = model;
  outer_uuids_[path] = model->uuid_;

  return model;
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

void AssetManager::RegisterModelImages(const ModelStorage &model_storage) {
  auto image_paths = model_storage.GetImagePaths();
  auto image_create_information = ImageCreateInformation::CreateTexture2D(0, 0, Walle::Format::E_R8G8B8A8_UNORM, 1);
  std::size_t total_size = 0;
  std::vector<ImageWrapper> image_wrappers;
  for (const auto &image_path : image_paths) {
    auto &image_wrapper = image_wrappers.emplace_back(image_path);
    auto image_data = image_wrapper.GetData();
    total_size += image_data.size_bytes();
  }

  std::size_t offset = 0;
  Buffer staging_buffer(BufferCreateInformation::CreateStagingBuffer(total_size));
  for (const auto &image_wrapper : image_wrappers) {
    auto image_data = image_wrapper.GetData();
    staging_buffer.SetData(image_data, offset);
    offset += image_data.size_bytes();
  }

  offset = 0;
  CommandBuffer command_buffer(GraphicsContext::Get()->GetGraphicsQueueIndex());
  command_buffer.Begin(Walle::CommandBufferUsageMaskBits::E_ONE_TIME_SUBMIT_BIT);
  for (const auto &image_wrapper : image_wrappers) {
    image_create_information.extent_.width = image_wrapper.GetWidth();
    image_create_information.extent_.height = image_wrapper.GetHeight();
    auto image = MakeIntrusivePointer<Image>(image_create_information, SamplerCreateInformation());
    assets_[image->uuid_] = image;
    image->SetImageLayout(Walle::ImageLayout::E_TRANSFER_DST_OPTIMAL, &command_buffer);
    command_buffer.CommandCopyBufferToImage(staging_buffer.GetHandle(), image->GetImage(), image->GetImageCreateInformation().extent_, offset);
    image->SetImageLayout(Walle::ImageLayout::E_SHADER_READ_ONLY_OPTIMAL, &command_buffer);
    offset += image_wrapper.GetData().size_bytes();
  }
  command_buffer.End();
  command_buffer.Submit();
}

const std::unordered_map<UUID, IntrusivePointer<Asset>> &AssetManager::GetAssets() const {
  return assets_;
}

} // namespace Yuggoth
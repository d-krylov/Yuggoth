#include "fastgltf/glm_element_traits.hpp"
#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include "fastgltf/tools.hpp"
#include "model_storage.h"

namespace Yuggoth {

namespace fgf = fastgltf;

Filter GetFilter(fgf::Filter filter) {
  switch (filter) {
  case fgf::Filter::Nearest:
  case fgf::Filter::NearestMipMapNearest:
  case fgf::Filter::NearestMipMapLinear: return Filter::E_NEAREST;
  case fgf::Filter::Linear:
  case fgf::Filter::LinearMipMapNearest:
  case fgf::Filter::LinearMipMapLinear: return Filter::E_LINEAR;
  default: std::unreachable();
  }
}

SamplerAddressMode GetWrap(fgf::Wrap wrap) {
  switch (wrap) {
  case fgf::Wrap::ClampToEdge: return SamplerAddressMode::E_CLAMP_TO_EDGE;
  case fgf::Wrap::MirroredRepeat: return SamplerAddressMode::E_MIRRORED_REPEAT;
  case fgf::Wrap::Repeat: return SamplerAddressMode::E_REPEAT;
  default: std::unreachable();
  }
}

SamplerSpecification GetSamplerSpecification(const fgf::Sampler &sampler) {
  SamplerSpecification specification;
  specification.min_filter_ = GetFilter(sampler.minFilter.value_or(fgf::Filter::Linear));
  specification.mag_filter_ = GetFilter(sampler.magFilter.value_or(fgf::Filter::Linear));
  specification.address_mode_u_ = GetWrap(sampler.wrapS);
  specification.address_mode_v_ = GetWrap(sampler.wrapT);
  return specification;
}

void GetModelProperties(const fgf::Asset &asset, uint32_t &vertices_count, uint32_t &indices_count, uint32_t &primitives_count) {
  for (const auto &mesh : asset.meshes) {
    for (const auto &primitive : mesh.primitives) {
      const auto position_attribute = primitive.findAttribute("POSITION");
      const auto &position_accessor = asset.accessors[position_attribute->accessorIndex];
      const auto &indices_accessor = asset.accessors[primitive.indicesAccessor.value()];
      vertices_count += position_accessor.count;
      indices_count += indices_accessor.count;
      primitives_count++;
    }
  }
}

auto LoadIndices(const fgf::Asset &asset, std::size_t accessor_index, std::size_t ioffset, std::size_t voffset, std::span<uint32_t> out) {
  const auto &indices_accessor = asset.accessors[accessor_index];
  auto indices_count = indices_accessor.count;
  auto indices_begin = out.begin() + ioffset;
  fgf::iterateAccessor<uint32_t>(asset, indices_accessor, [&](uint32_t index) { *indices_begin++ = index + voffset; });
  return ioffset + indices_count;
}

template <typename T, typename Function>
void LoadVertexData(const fgf::Asset &asset, const fgf::Primitive &primitive, std::string_view attribute_name, Function &&callback) {
  if (const auto &attribute = primitive.findAttribute(attribute_name); attribute != primitive.attributes.end()) {
    const auto &accessor = asset.accessors[attribute->accessorIndex];
    fgf::iterateAccessorWithIndex<T>(asset, accessor, std::forward<Function>(callback));
  }
}

template <typename T>
concept TextureInformationT = std::derived_from<T, fgf::TextureInfo>;

template <TextureInformationT T> int32_t LoadTexture(const fgf::Optional<T> &texture_information) {
  if (texture_information.has_value() == false) {
    return -1;
  }
  return texture_information->textureIndex;
}

auto LoadVertices(const fgf::Asset &asset, const fgf::Primitive &primitive, std::size_t vertices_offset, std::span<Vertex> out_vertices) {
  auto position_attribute = primitive.findAttribute("POSITION");
  const auto &position_accessor = asset.accessors[position_attribute->accessorIndex];
  auto set_position = [&](const Vector3f &position, std::size_t index) { out_vertices[vertices_offset + index].position_ = position; };
  auto set_texture = [&](const Vector2f &uv, std::size_t index) { out_vertices[vertices_offset + index].uv_ = uv; };
  auto set_normal = [&](const Vector3f &normal, std::size_t index) { out_vertices[vertices_offset + index].normal_ = normal; };
  fgf::iterateAccessorWithIndex<Vector3f>(asset, position_accessor, set_position);
  LoadVertexData<Vector3f>(asset, primitive, "NORMAL", set_normal);
  LoadVertexData<Vector2f>(asset, primitive, "TEXCOORD_0", set_texture);
  return vertices_offset + position_accessor.count;
}

void LoadPrimitives(const fgf::Asset &asset, std::span<Vertex> out_vertices, std::span<uint32_t> out_indices, std::span<Mesh> out_meshes) {
  std::size_t vertices_offset = 0, indices_offset = 0, primitive_index = 0;
  for (const auto &mesh : asset.meshes) {
    for (auto &primitive : mesh.primitives) {
      const auto &indices_accessor = asset.accessors[primitive.indicesAccessor.value()];
      auto material_index = primitive.materialIndex.value_or(-1);
      out_meshes[primitive_index++] = Mesh(material_index, vertices_offset, indices_offset, indices_accessor.count);
      indices_offset = LoadIndices(asset, primitive.indicesAccessor.value(), indices_offset, vertices_offset, out_indices);
      vertices_offset = LoadVertices(asset, primitive, vertices_offset, out_vertices);
    }
  }
}

void LoadImages(const fgf::Asset &asset, const std::filesystem::path &path) {
  for (const auto &image : asset.images) {
    auto image_name = std::get<fastgltf::sources::URI>(image.data).uri.path();
    auto image_path = path.parent_path() / image_name;
  }
}

MeshData ModelStorage::LoadKhronos(const std::filesystem::path &path) {

  auto extensions = fgf::Extensions::KHR_mesh_quantization | fgf::Extensions::KHR_texture_transform | fgf::Extensions::KHR_materials_variants;
  auto options = fgf::Options::DontRequireValidAssetMember | fgf::Options::LoadExternalBuffers | fgf::Options::GenerateMeshIndices;

  fastgltf::Parser parser(extensions);

  auto gltf_file = fastgltf::MappedGltfFile::FromPath(path);

  auto asset = parser.loadGltf(gltf_file.get(), path.parent_path(), options);

  uint32_t primitives_count = 0;
  GetModelProperties(asset.get(), number_of_vertices_, number_of_indices_, primitives_count);

  MeshData mesh_data;

  mesh_data.vertices_.resize(number_of_vertices_);
  mesh_data.indices_.resize(number_of_indices_);

  meshes_.resize(primitives_count);

  LoadPrimitives(asset.get(), mesh_data.vertices_, mesh_data.indices_, meshes_);

  for (const auto &image : asset->images) {
    auto image_name = std::get<fastgltf::sources::URI>(image.data).uri.path();
    images_.emplace_back(path.parent_path() / image_name);
  }

  for (const auto &material : asset->materials) {
    Material current_material;
    auto color = material.pbrData.baseColorFactor;
    current_material.color_texture_index_ = LoadTexture<fgf::TextureInfo>(material.pbrData.baseColorTexture);
    current_material.normal_texture_index_ = LoadTexture<fgf::NormalTextureInfo>(material.normalTexture);
    current_material.emissive_texture_index_ = LoadTexture<fgf::TextureInfo>(material.emissiveTexture);
    current_material.base_color_ = Vector4f(color.x(), color.y(), color.z(), color.w());
    materials_.emplace_back(current_material);
  }

  for (const auto &sampler : asset->samplers) {
    sampler_specifications_.emplace_back(GetSamplerSpecification(sampler));
  }

  for (const auto &texture : asset->textures) {
    Texture current_texture;
    current_texture.sampler_index_ = texture.samplerIndex.value_or(-1);
    current_texture.image_index_ = texture.imageIndex.value_or(-1);
  }

  return mesh_data;
}

} // namespace Yuggoth
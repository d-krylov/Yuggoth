#include "fastgltf/glm_element_traits.hpp"
#include "fastgltf/core.hpp"
#include "fastgltf/types.hpp"
#include "fastgltf/tools.hpp"
#include <filesystem>

namespace Yuggoth {

void LoadGLTF(const std::filesystem::path &path) {

  auto extensions = fastgltf::Extensions::KHR_mesh_quantization | //
                    fastgltf::Extensions::KHR_texture_transform | //
                    fastgltf::Extensions::KHR_materials_variants; //

  auto options = fastgltf::Options::DontRequireValidAssetMember | //
                 fastgltf::Options::LoadExternalBuffers |         //
                 fastgltf::Options::GenerateMeshIndices;

  fastgltf::Parser parser(extensions);

  auto gltf_file = fastgltf::MappedGltfFile::FromPath(path);

  auto assets = parser.loadGltf(gltf_file.get(), path.parent_path(), options);
}

} // namespace Yuggoth
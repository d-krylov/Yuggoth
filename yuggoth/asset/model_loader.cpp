#include "yuggoth/asset/include/model_loader.h"

namespace Yuggoth {

void ModelLoader::Load(const std::filesystem::path &path) {
  auto extension = path.extension();
  if (extension == ".gltf") {
    LoadKhronos(path);
  }
}

std::span<const Vertex> ModelLoader::GetVertices() const {
  return vertices_;
}

std::span<const uint32_t> ModelLoader::GetIndices() const {
  return indices_;
}

std::size_t ModelLoader::GetNumberOfVertices() {
  return vertices_.size();
}

std::size_t ModelLoader::GetNumberOfIndices() {
  return indices_.size();
}

} // namespace Yuggoth
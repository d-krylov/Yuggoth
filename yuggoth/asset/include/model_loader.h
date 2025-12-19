#ifndef YUGGOTH_MODEL_LOADER_H
#define YUGGOTH_MODEL_LOADER_H

#include "mesh.h"
#include <filesystem>

namespace Yuggoth {

class ModelLoader {
public:
  void Load(const std::filesystem::path &path);

  std::span<const Vertex> GetVertices() const;
  std::span<const uint32_t> GetIndices() const;

  std::size_t GetNumberOfVertices();
  std::size_t GetNumberOfIndices();

protected:
  void LoadKhronos(const std::filesystem::path &path);
  void LoadWavefront(const std::filesystem::path &path);

private:
  std::vector<Vertex> vertices_;
  std::vector<uint32_t> indices_;
  std::vector<std::filesystem::path> textures_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_LOADER_H
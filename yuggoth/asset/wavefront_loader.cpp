#include "yuggoth/asset/include/model_loader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

namespace Yuggoth {

MeshData ModelLoader::LoadWavefront(const std::filesystem::path &path) {
  tinyobj::ObjReader reader;
  tinyobj::ObjReaderConfig reader_config;

  auto status = reader.ParseFromFile(path.string(), reader_config);
}

} // namespace Yuggoth
#include "yuggoth/asset/include/model.h"
#include "yuggoth/graphics/core/graphics_structures.h"

namespace Yuggoth {

Model::Model(const std::filesystem::path &model_path) : model_path_(model_path) {
  auto extension = model_path_.extension();
  if (extension == ".gltf") {
    LoadKhronos(model_path_);
  }
}

const std::filesystem::path &Model::GetModelPath() const {
  return model_path_;
}

const BufferInformation &Model::GetVerticesInformation() const {
  return vertices_;
}

const BufferInformation &Model::GetIndicesInformation() const {
  return indices_;
}

DrawIndexedIndirectCommand Model::GetDrawCommand() const {
  DrawIndexedIndirectCommand command;
  command.firstIndex = indices_.buffer_offset_;
  command.firstInstance = 0;
  command.indexCount = indices_.buffer_count_;
  command.instanceCount = 1;
  command.vertexOffset = vertices_.buffer_offset_;
  return command;
}

} // namespace Yuggoth
#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "asset.h"
#include "mesh.h"

namespace Yuggoth {

struct DrawIndexedIndirectCommand;

class Model : public Asset {
public:
  Model(const std::filesystem::path &model_path);

  const std::filesystem::path &GetModelPath() const;

  const BufferInformation &GetVerticesInformation() const;
  const BufferInformation &GetIndicesInformation() const;

  DrawIndexedIndirectCommand GetDrawCommand() const;

protected:
  void LoadKhronos(const std::filesystem::path &path);
  void LoadWavefront(const std::filesystem::path &path);

private:
  uint32_t vertices_count_{0};
  uint32_t indices_count_{0};
  BufferInformation vertices_;
  BufferInformation indices_;
  std::filesystem::path model_path_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
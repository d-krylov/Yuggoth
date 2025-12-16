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

  const BufferRangeInformation &GetVerticesInformation() const;
  const BufferRangeInformation &GetIndicesInformation() const;

  void SetVerticesInformation(const BufferRangeInformation &vertices);
  void SetIndicesInformation(const BufferRangeInformation &indices);

private:
  BufferRangeInformation vertices_;
  BufferRangeInformation indices_;
  std::filesystem::path model_path_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
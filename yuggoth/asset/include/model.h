#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "asset.h"
#include "mesh.h"
#include <filesystem>
#include "yuggoth/core/tools/include/core_types.h"

namespace Yuggoth {

class BottomLevelGeometry;

class Model : public Asset {
public:
  Model(const std::filesystem::path &model_path);

  const std::filesystem::path &GetModelPath() const;

  const BufferRange &GetVerticesInformation() const;
  const BufferRange &GetIndicesInformation() const;

  void SetVerticesInformation(const BufferRange &vertices);
  void SetIndicesInformation(const BufferRange &indices);

  BottomLevelGeometry GetBottomLevelGeometry() const;

  AssetKind GetAssetKind() const override;

private:
  BufferRange vertices_;
  BufferRange indices_;
  std::filesystem::path model_path_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
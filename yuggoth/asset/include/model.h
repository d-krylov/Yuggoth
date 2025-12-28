#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "model_base.h"
#include <filesystem>

namespace Yuggoth {

class BottomLevelGeometry;

class Model : public ModelBase {
public:
  Model(const std::filesystem::path &model_path);

  const std::filesystem::path &GetModelPath() const;

  BufferRange GetVertexBufferRange() const override;
  BufferRange GetIndexBufferRange() const override;

  void SetVertexBufferRange(const BufferRange &vertices);
  void SetIndexBufferRange(const BufferRange &indices);

  BottomLevelGeometry GetBottomLevelGeometry() const;

  AssetKind GetAssetKind() const override;

private:
  BufferRange vertices_;
  BufferRange indices_;
  std::filesystem::path model_path_;
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
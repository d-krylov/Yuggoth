#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "asset.h"
#include "mesh.h"

namespace Yuggoth {

class Model : public Asset {
public:
  Model(const std::filesystem::path &model_path);

protected:
  void LoadKhronos(const std::filesystem::path &path);
  void LoadWavefront(const std::filesystem::path &path);

private:
  std::size_t vertices_count_{0};
  std::size_t indices_count_{0};
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
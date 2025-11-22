#ifndef YUGGOTH_MODEL_H
#define YUGGOTH_MODEL_H

#include "asset.h"
#include "mesh.h"

namespace Yuggoth {

class Model : public Asset {
public:
  Model(const std::filesystem::path &model_path);

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_MODEL_H
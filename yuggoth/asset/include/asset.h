#ifndef YUGGOTH_ASSET_H
#define YUGGOTH_ASSET_H

#include "yuggoth/core/tools/include/uuid.h"
#include <filesystem>

namespace Yuggoth {

enum class AssetKind {
  UNKNOWN,
  MODEL,
  IMAGE
};

class Asset {
public:
  AssetKind kind_;
  UUID uuid_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_H
#ifndef YUGGOTH_ASSET_H
#define YUGGOTH_ASSET_H

#include "yuggoth/core/tools/include/uuid.h"
#include "yuggoth/core/intrusive/intrusive_reference_counter.h"

namespace Yuggoth {

enum class AssetKind {
  UNKNOWN,
  MODEL,
  IMAGE
};

class Asset : public IntrusiveReferenceCounter<Asset> {
public:
  virtual AssetKind GetAssetKind() const {
    return AssetKind::UNKNOWN;
  }

  UUID uuid_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_H
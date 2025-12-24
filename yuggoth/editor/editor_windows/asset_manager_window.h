#ifndef YUGGOTH_ASSET_MANAGER_WINDOW_H
#define YUGGOTH_ASSET_MANAGER_WINDOW_H

#include "yuggoth/editor/core/editor_window.h"

namespace Yuggoth {

class AssetManagerWindow : public EditorWindow {
public:
  AssetManagerWindow();

  void OnImGui();

protected:
  void DrawAssets();

private:
  Image2D folder_image_;
};

} // namespace Yuggoth

#endif // YUGGOTH_ASSET_MANAGER_WINDOW_H
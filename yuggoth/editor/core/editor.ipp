#ifndef YUGGOTH_EDITOR_IPP
#define YUGGOTH_EDITOR_IPP

#include <concepts>

namespace Yuggoth {

template <typename T> T &Editor::GetEditorWindow() {
  if constexpr (std::same_as<T, ViewportWindow>) {
    return viewport_window_;
  } else if constexpr (std::same_as<T, HierarchyWindow>) {
    return hierarchy_window_;
  } else if constexpr (std::same_as<T, InspectorWindow>) {
    return inspector_window_;
  } else if constexpr (std::same_as<T, ApplicationWindow>) {
    return application_window_;
  } else {
    static_assert(false, "Window type not registered in Editor");
  }
}

} // namespace Yuggoth

#endif // YUGGOTH_EDITOR_IPP
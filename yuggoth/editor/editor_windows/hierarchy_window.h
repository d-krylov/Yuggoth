#ifndef YUGGOTH_HIERARCHY_WINDOW_H
#define YUGGOTH_HIERARCHY_WINDOW_H

namespace Yuggoth {

class HierarchyWindow {
public:
  void OnImGui();

protected:
  void AddEntity();
  void DrawNodes();

private:
};

} // namespace Yuggoth

#endif // YUGGOTH_HIERARCHY_WINDOW_H
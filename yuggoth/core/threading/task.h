#ifndef YUGGOTH_TASK_H
#define YUGGOTH_TASK_H

#include <functional>

namespace Yuggoth {

class Task {
public:
  using TaskType = std::function<void()>;

private:
  TaskType task_;
};

} // namespace Yuggoth

#endif // YUGGOTH_TASK_H

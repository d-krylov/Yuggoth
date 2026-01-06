#ifndef YUGGOTH_TASK_SYSTEM_H
#define YUGGOTH_TASK_SYSTEM_H

#include <thread>
#include <vector>

namespace Yuggoth {

class TaskSystem {
public:
  TaskSystem();

  void Start();

private:
  std::vector<std::jthread> threads_;
};

} // namespace Yuggoth

#endif // YUGGOTH_TASK_SYSTEM_H
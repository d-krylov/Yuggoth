#ifndef YUGGOTH_TASK_SYSTEM_H
#define YUGGOTH_TASK_SYSTEM_H

#include <thread>
#include <vector>
#include <yuggoth/core/allocators/object_pool.h>
#include "task.h"

namespace Yuggoth {

class TaskSystem {
public:
  TaskSystem();

  static TaskSystem *Get();

  void Start();

  template <typename Function> void CreateTask(Function &&function);

private:
  std::vector<std::jthread> threads_;
  std::vector<Task *> pending_tasks_;

  std::atomic_uint completed_tasks_;

  static TaskSystem *task_system_instance_;
};

} // namespace Yuggoth

#include "task_system.ipp"

#endif // YUGGOTH_TASK_SYSTEM_H
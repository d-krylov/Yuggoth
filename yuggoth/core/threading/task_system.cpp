#include "task_system.h"
#include <cassert>

namespace Yuggoth {

TaskSystem *TaskSystem::task_system_instance_ = nullptr;

TaskSystem *TaskSystem::Get() {
  assert(task_system_instance_ != nullptr);
  return task_system_instance_;
}

TaskSystem::TaskSystem() {

  task_system_instance_ = this;
}

void TaskSystem::Start() {
}

} // namespace Yuggoth
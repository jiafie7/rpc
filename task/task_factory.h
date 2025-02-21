#pragma once

#include "task/work_task.h"
using namespace melon::task;

namespace melon
{
  namespace task
  {
    class TaskFactory
    {
    public:
      static Task* create(int socket_fd)
      {
        return new WorkTask(socket_fd);
      }
    };
  }
}

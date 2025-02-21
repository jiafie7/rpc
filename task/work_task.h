#pragma once

#include <cstdint> 
#include "thread/task.h"
using namespace melon::thread;

namespace melon
{
  namespace task
  {
    // message head, 4 bytes
    struct MsgHead
    {
      uint16_t cmd;  // work id
      uint16_t len;  // length of message body
    };

    const uint32_t recv_buf_size = 1024;

    class WorkTask : public Task
    {
    public:
      WorkTask() = delete;
      WorkTask(int socket_fd);
      ~WorkTask();

      virtual void run();
      virtual void destroy();
        
    private:
      int m_socket_fd = 0;
      bool m_closed = false;
    };
  }
}

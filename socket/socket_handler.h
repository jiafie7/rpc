#pragma once

#include "socket/server_socket.h"
#include "socket/event_poller.h"

namespace melon
{
  namespace socket
  {
    class SocketHandler
    {
      SINGLETON(SocketHandler);
    public:
      void listen(const std::string& ip, int port);
      void attach(int socket_fd);
      void detach(int socket_fd);
      void handle(int max_conn, int timeout);

    private:
      Socket* m_server = nullptr;
      EventPoller m_epoller;
    };
  }
}

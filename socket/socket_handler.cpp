#include "socket/socket_handler.h"
using namespace melon::socket;

#include "task/task_factory.h"
using namespace melon::task;

#include "thread/task_dispatcher.h"
using namespace melon::thread;

void SocketHandler::listen(const std::string& ip, int port)
{
  m_server = new ServerSocket(ip, port);
}

void SocketHandler::attach(int socket_fd)
{
  m_epoller.addFd(socket_fd, EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLONESHOT);
}

void SocketHandler::detach(int socket_fd)
{
  m_epoller.delFd(socket_fd);
}

void SocketHandler::handle(int max_conn, int timeout)
{
  m_epoller.create(max_conn);
  m_epoller.addFd(m_server->fd(), EPOLLIN | EPOLLHUP | EPOLLERR);

  while (true)
  {
    int ret = m_epoller.wait(timeout);
    if (ret < 0)
    {
      log_error("epoll wait error: errno = %d, errmsg = %s.", errno, strerror(errno));
      break;
    }
    else if (ret == 0)
    {
      log_debug("epoll wait timeout.");
      continue;
    }
    log_debug("epoll wait ok: ret = %d.", ret);

    for (int i = 0; i < ret; ++ i)
    {
      if (m_epoller.getFd(i) == m_server->fd())
      {
        int conn_fd = m_server->accept();
        if (conn_fd < 0)
        {
          log_error("server accept error: errno = %d, errmsg = %s.", errno, strerror(errno));
          continue;
        }

        Socket socket(conn_fd);
        socket.setNonBlocking();

        attach(conn_fd);
      }
      else
      {
        int fd = m_epoller.getFd(i);
        if (m_epoller.isSet(i, EPOLLHUP))
        {
          log_error("socket hang up by peer: conn = %d, errno = %d, errmsg = %s.", fd, errno, strerror(errno));
          detach(fd);
          ::close(fd);
        }
        else if (m_epoller.isSet(i, EPOLLERR))
        {
          log_error("socket error: conn = %d, errno = %d, errmsg = %s.", fd, errno, strerror(errno));
          detach(fd);
          ::close(fd);
        }
        else if (m_epoller.isSet(i, EPOLLIN))
        {
          detach(fd);

          auto task = TaskFactory::create(fd);
          Singleton<TaskDispatcher>::getInstance()->push(task);
        }
      }
    }
  }
}

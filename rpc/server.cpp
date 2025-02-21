#include "rpc/server.h"
using namespace melon::rpc;

#include "frame/server.h"

void Server::start()
{
  try 
  {
    Singleton<melon::frame::Server>::getInstance()->start();
  }
  catch (std::exception& e)
  {
    std::cout << e.what() << '\n';
  }
}

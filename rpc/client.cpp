#include "rpc/client.h"
using namespace melon::rpc;

#include "utility/system.h"
#include "utility/log_system.h"
using namespace melon::utility;

Client::Client(const std::string& ip, int port)
  : ClientSocket(ip, port)
{
  System* sys = Singleton<System>::getInstance();
  sys->init(); 
  std::string root_path = sys->getRootPath();
  
  LogSystem* log_system = Singleton<LogSystem>::getInstance();
  log_system->open(root_path + "/log/client.log");
}

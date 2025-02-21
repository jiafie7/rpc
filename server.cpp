#include <iostream>
#include <string>

#include "rpc/server.h"
using namespace melon::rpc;

int add(int x, int y)
{
  return x + y;
}

std::string hello(const std::string& name)
{
  return "hello, " + name;
}

int main()
{  
  Server* server = Singleton<Server>::getInstance();
  server->bind("add", add);
  server->bind("hello", hello);
  server->start();
  
  return 0;
}

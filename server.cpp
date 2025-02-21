#include <iostream>

#include "rpc/server.h"
using namespace melon::rpc;

int add(int x, int y)
{
  return x + y;
}

int main()
{  
  Server* server = Singleton<Server>::getInstance();
  server->bind("add", add);
  server->start();
  
  return 0;
}

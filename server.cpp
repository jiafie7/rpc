#include <algorithm>
#include <iostream>
#include <string>

#include "rpc/server.h"
using namespace melon::rpc;

#include "test/request.h"
#include "test/response.h"
using namespace melon::test;

int add(int x, int y)
{
  return x + y;
}

std::string hello(const std::string& name)
{
  return "hello, " + name;
}

Response toUpper(const Request& req)
{
  std::string name = req.getName();
  std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) {
    return std::toupper(c);
  });
  return Response(name);
}

int main()
{  
  Server* server = Singleton<Server>::getInstance();
  server->bind("add", add);
  server->bind("hello", hello);
  server->bind("toUpper", toUpper);
  server->start();
  
  return 0;
}

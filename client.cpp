#include <iostream>
#include <string>

#include "rpc/client.h"
using namespace melon::rpc;

#include "test/request.h"
#include "test/response.h"
using namespace melon::test;

int main()
{
  Client client("127.0.0.1", 7777);

  int sum = client.call<int>("add", 3, 7);
  std::cout << sum << '\n';

  std::string str = client.call<std::string>("hello", "Michael");
  std::cout << str << '\n';
   
  Request req("nvidia");
  Response resp = client.call<Response>("toUpper", req);
  std::cout << resp.getName() << '\n';

  client.close();

  return 0;
}

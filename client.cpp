#include <iostream>

#include "rpc/client.h"
using namespace melon::rpc;

int main()
{
  Client client("127.0.0.1", 7777);

  int sum = client.call<int>("add", 3, 7);
  std::cout << sum << '\n';

  client.close();

  return 0;
}

#include <iostream>

#include "serialize/data_stream.h"
using namespace melon::serialize;

#include "rpc/function.h"
using namespace melon::rpc;

int add(int x, int y)
{
  return x + y;
}

int main()
{
  // bind function name with func
  Function f;
  f.bind("add", add);

  // serialize function name and arguments
  DataStream in;
  in << "add" << 3 << 7;

  // unserialize function name
  std::string func_name;
  in >> func_name;

  // execute function
  DataStream out;
  f.call(func_name, in, out);

  // unserialize function return results
  int sum;
  out >> sum;
  std::cout << "3 + 7 = " << sum << '\n';

  return 0;
}

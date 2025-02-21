#pragma once

#include "rpc/function.h"

#include "utility/singleton.h"
using namespace melon::utility;

#include "serialize/data_stream.h"
using namespace melon::serialize;

namespace melon
{
  namespace rpc
  {
    class Server
    {
      SINGLETON(Server);
    public:
      void start(); 

      template <typename F>
      void bind(const std::string& name, F func)
      {
        m_func.bind(name, func);
      }

      void call(const std::string& name, DataStream& in, DataStream& out)
      {
        m_func.call(name, in, out);
      }

    private:
      Function m_func;
    };
  }
}

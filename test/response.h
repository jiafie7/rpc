#pragma once

#include <string>
#include "serialize/serializable.h"
using namespace melon::serialize;

namespace melon
{
  namespace test
  {
    class Response : public Serializable
    {
      public:
        Response() = default;
        Response(const std::string& name)
          : m_name(name)
        {}
        ~Response() = default;

        const std::string& getName() const
        {
          return m_name;
        }

        SERIALIZE(m_name);

      private:
        std::string m_name;
    };
  }
}



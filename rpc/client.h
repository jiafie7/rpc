#pragma once

#include "socket/client_socket.h"
using namespace melon::socket;

#include "serialize/data_stream.h"
using namespace melon::serialize;

namespace melon
{
  namespace rpc
  {
    // custom message protocol: Fixed message header + Variable-length message body
    struct MsgHead
    {
      uint16_t cmd;
      uint16_t len;
    };

    const uint32_t send_buff_size = 1024;
    const uint32_t recv_buff_size = 1024;

    class Client : public ClientSocket
    {
    public:
      Client() = delete;
      Client(const std::string& ip, int port);
      ~Client() = default;

      template <typename R, typename... Args>
      R call(const std::string& name, const Args&... args);
    };

    template <typename R, typename... Args>
    R Client::call(const std::string& name, const Args&... args)
    {
      DataStream in;
      /***** 1. serialize message body *****/
      in << name;
      in.write_args(args...);


      /***** 2. send serializing data to server *****/
      // construct message head
      MsgHead head;
      head.cmd = 0; 
      head.len = in.size();

      char send_buff[send_buff_size] = {0};
      // move message head to send buffer
      std::memcpy(send_buff, (char*)&head, sizeof(MsgHead));
      // move message body to send buffer
      std::memcpy(send_buff + sizeof(MsgHead), in.data(), in.size());

      int len = sizeof(MsgHead) + in.size();
      send(send_buff, len); 


      /***** 3. recv serializing data from server *****/
      char recv_buff[recv_buff_size] = {0};
      int recv_size = recv(recv_buff, recv_buff_size);
  
      DataStream out;
      out.write(recv_buff, recv_size);


      /***** 4. unserialize data *****/
      R v;
      out >> v;

      return v;
    }
  }
}

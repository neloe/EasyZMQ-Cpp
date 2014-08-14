/*!
 * \file message.h
 * \author Nathan ELoe
 * \brief A basic message class
 */

#include "_base_msg.h"
#include "../socket.h"

namespace zmqcpp
{
  class Message: public BaseMessage<Message>
  {
    public:
      bool send(zmqcpp::Socket & sock, const int opts = 0) {return _gen_send(sock, opts);}
  };
}
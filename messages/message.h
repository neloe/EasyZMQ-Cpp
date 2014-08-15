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
    friend class BaseMessage<Message>;
    protected:
      std::list <std::shared_ptr<std::string>> & prep_frames() {return m_frames;}
      void unprep_frames() {}
      void start_recv() {}
      bool recv_more() {return false;}
  };
}
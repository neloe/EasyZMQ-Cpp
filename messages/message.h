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
      /*!
       * \brief prepares the frames to be sent
       * \pre None
       * \post None
       * \returns the prepared frames to send
       */
      std::list <std::shared_ptr<std::string>> & prep_frames() {return m_frames;}
      /*!
       * \brief cleans up after sending the frames
       * \pre None
       * \post None
       */
      void unprep_frames() {return;}
      /*!
       * \brief Prepares to receive
       * \pre None
       * \post None
       */
      void start_recv() {return;}
      /*!
       * \brief Determines whether or not more frames should be received
       * \pre None
       * \post None
       * \returns false (only one frame should be received)
       */
      bool recv_more() {return false;}
  };
}
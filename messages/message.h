/*!
 * \file message.h
 * \author Nathan ELoe
 * \brief A basic message class
 */
#pragma once

#include <sstream>

#include "_base_msg.h"
#include "../socket.h"

namespace zmqcpp
{
  class Message: public BaseMessage<Message>
  {
    friend class BaseMessage<Message>;
    public:
      /*!
       * \brief Default constructor
       */
      Message() = default;
      /*!
       * \brief Construction from other type
       * \pre type T has stringstream << T defined; that results in a good string representation
       * \post Object is constructed with one frame: the data
       */
      template <class T>
      Message (const T& data);
    protected:
      /*!
       * \brief prepares the frames to be sent
       * \pre None
       * \post None
       * \returns the prepared frames to send
       */
      std::list <std::shared_ptr<std::string>> prep_frames() const {return m_frames;}
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
  
  template <class T>
  Message::Message(const T& data)
  {
    std::stringstream ss;
    ss << data;
    add_frame(ss.str());
  }
}
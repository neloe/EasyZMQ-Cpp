/*
  Copyright (c) Nathan Eloe, 2014
  This file is part of EasyZMQ-Cpp.

  EasyZMQ-Cpp is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  EasyZMQ-Cpp is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with EasyZMQ-Cpp.  If not, see <http://www.gnu.org/licenses/>.
*/
/*!
 * \file message_base.h
 * \author Nathan Eloe
 * \brief A templated base class (using the BNT) to implement magic
 */

#pragma once
#include <list>
#include <memory>
#include <string>
#include <cstring>

#include "../socket.h"

namespace zmqcpp
{  
  template <class T>
  class BaseMessage
  {
    protected:
      friend class Socket; 
      std::list <std::shared_ptr<std::string>> m_frames;
      bool m_rstart;
      ///@{
      /*!
       * \brief The magic behind the barton-nackman trick
       * \pre None
       * \post None
       * \returns this object as it's child class
       */
      T& as_child() {return static_cast<T&> (*this);}
      const T& as_child() const {return static_cast<const T&> (*this);}
      ///@}
      // ------- Protected functions under this line MUST be implemented in child classes
      /*!
       * \brief prepares the frames to be sent
       * \pre The child class has the prep_frames() function implemented
       * \post Calls the child's prep_frames function
       * \returns the prepared frames to send
       */
      const std::list <std::shared_ptr<std::string>> prep_frames() const {return as_child().prep_frames();}
      /*!
       * \brief cleans up after sending the frames
       * \pre The child class has the unprep_frames() function implemented
       * \post Calls the child's prep_frames function
       */
      void unprep_frames() {as_child().unprep_frames();}
      
      /*!
       * \brief Resets the marker for the beginning of the receive functionality
       * \pre The child class has the start_recv() function implemented
       * \post Sets the recv start flag to true, calls the child's start_recv() function
       */
      void start_recv() {m_rstart = true; as_child().start_recv();}
      /*!
       * \brief Determines whether or not more frames should be received
       * \pre The child class has the recv_more() function implemented
       * \post calls the child's recv_more() function, sets the recv_start flag to false
       * \returns Whether or not to return more (result of child's recv_more function)
       */
      bool recv_more() {bool more = as_child().recv_more(); m_rstart = false; return more;}

    public:
      ///@{
      /*!
       * \brief Adds a frame to the message
       * \pre None
       * \post the string or sequence of bytes is added to the frames to send
       */
      void add_frame(const std::string & s) {m_frames.push_back(std::shared_ptr<std::string>(new std::string(s)));}
      void add_frame(const char* bytes, const int size = -1) {add_frame(std::string(bytes, (size == -1?strlen(bytes) : size)));}
      ///@}
      /*!
       * \brief combination of two messages
       * \pre None
       * \post takes the frames of the second message and appends it/them to this message's frames
       * \post Destroys the other message
       * \returns This message after the combination
       */
      T& operator += (BaseMessage<T>& msg) {m_frames.splice(m_frames.end(), msg.m_frames); return as_child();}
      /*!
       * \brief Returns the list of frames
       * \pre None
       * \post None
       * \returns the list of frames
       */
      const std::list<std::shared_ptr<std::string>> frames() const {return m_frames;}
            
      /*!
       * \brief returns the last string in the message list
       * \pre None
       * \post None
       * \returns Last message if >=1 frame, else empty string
       */
      std::string last() {return (m_frames.size())? *(m_frames.back()): "";}
      
      /*!
       * \brief empties the frame list
       * \pre None
       * \post the frame list is made cleared 
       */
      void clear() {m_frames.clear();}
  };
}

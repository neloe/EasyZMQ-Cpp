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
 * \file message.h
 * \author Nathan Eloe
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
    Message (const T &data);
  protected:
    /*!
     * \brief prepares the frames to be sent
     * \pre None
     * \post None
     * \returns the prepared frames to send
     */
    const std::list <std::shared_ptr<std::string>> &prep_frames() const
    {
        return m_frames;
    }
    /*!
     * \brief cleans up after sending the frames
     * \pre None
     * \post None
     */
    void unprep_frames() const
    {
        return;
    }
    /*!
     * \brief Prepares to receive
     * \pre None
     * \post None
     */
    void start_recv()
    {
        return;
    }
    /*!
     * \brief signifies the end of recv
     * \pre None
     * \post None
     */
    void end_recv() {}
};

template <class T>
Message::Message (const T &data)
{
    std::stringstream ss;
    ss << data;
    add_frame (ss.str());
}
}
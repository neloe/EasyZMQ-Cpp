/*!
 * \file _base_msg.cpp
 * \author Nathan Eloe
 * \brief implementation of the base message type
 */

#pragma once
#include <zmq.hpp>

#include "_base_msg.h"
#include "../socket.h"

namespace zmqcpp
{
  template <class T>
  bool BaseMessage<T>::_gen_send(zmqcpp::Socket & sock, const int opts)
  {
    int count = 1;
    bool win = true;
    zmq::message_t z_msg;

    for (auto s: m_frames)
    {
      if (count < m_frames.size())
      {
	z_msg.rebuild(s->c_str(), s->size(), strp_free);
	win &= sock.raw_sock().send(z_msg, opts & ZMQ_SNDMORE);
	count ++;
      }
    }
    //now send the last frame without enforcing the SNDMORE flag
    z_msg.rebuild(m_frames.back()->c_str(), m_frames.back()->size(), strp_free);
    win &= sock.raw_sock().send(z_msg, opts & ZMQ_SNDMORE);
    return win;
  }
}
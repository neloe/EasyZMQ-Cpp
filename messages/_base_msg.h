/*!
 * \file message_base.h
 * \author Nathan Eloe
 * \brief A templated base class (using the BNT) to implement magic
 */

#pragma once
#include <list>
#include <memory>
#include <string>

#include "../socket.h"

namespace zmqcpp
{  
  template <class T>
  class BaseMessage
  {
    protected:
      std::list <std::shared_ptr<std::string>> m_frames;
      
      T& as_child() {return static_cast<T&> (*this);}
      void strp_free(void *, void *) {return;}
      bool _gen_send(zmqcpp::Socket & sock, const int opts = 0);
    public:
      void add_frame(const std::string & s) {m_frames.push_back(std::make_shared<std::string>(s));}
      void add_frame(const char* bytes, const int size = -1);
      T& operator += (BaseMessage<T>& msg) {m_frames.splice(m_frames.end(), msg.m_frames); return as_child();}
      bool send(zmqcpp::Socket & sock, const int opts = 0) const {return as_child().send(sock, opts);}
      
  };
}

#include "_base_msg.hpp"
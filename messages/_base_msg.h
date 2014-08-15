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
    public:
      void add_frame(const std::string & s) {m_frames.push_back(std::make_shared<std::string>(s));}
      void add_frame(const char* bytes, const int size = -1);
      T& operator += (BaseMessage<T>& msg) {m_frames.splice(m_frames.end(), msg.m_frames); return as_child();}
      std::list <std::shared_ptr<std::string>> & prep_frames() {return as_child().prep_frames();}
      void unprep_frames() {as_child().unprep_frames();}
      
  };
}

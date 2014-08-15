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
      T& as_child() {return static_cast<T&> (*this);}
      
      std::list <std::shared_ptr<std::string>> & prep_frames() {return as_child().prep_frames();}
      void unprep_frames() {as_child().unprep_frames();}
      
      void start_recv() {m_rstart = true; as_child().start_recv();}
      bool recv_more() {bool more = as_child().recv_more(); m_rstart = false; return more;}
    public:
      void add_frame(const std::string & s) {m_frames.push_back(std::make_shared<std::string>(s));}
      void add_frame(const char* bytes, const int size = -1) {add_frame(std::string(bytes, (size == -1?strlen(bytes) : size)));}
      T& operator += (BaseMessage<T>& msg) {m_frames.splice(m_frames.end(), msg.m_frames); return as_child();}
      const std::list<std::shared_ptr<std::string>> frames() const {return m_frames;}
  };
}

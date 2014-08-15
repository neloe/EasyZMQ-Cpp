/*!
 * \file socket.h
 * \author Nathan Eloe
 * \brief Definition of a quasi-singleton socket class
 */

#pragma once

#include <exception>
#include <map>
#include <memory>
#include <string>
#include <zmq.hpp>
#include "messages/_base_msg.h"

namespace zmqcpp
{
  class no_endpt : public std::exception
  {
    public:
      const char* what() {return "Socket has no endpoint";}
  };
  
  class Socket
  {
    private:
      // connected sockets are thread_local, but static by connection string
      static thread_local std::map<std::string, std::shared_ptr<zmq::socket_t>> m_conn;
      // bound sockets are static (not thread_local)
      static std::map<std::string, std::shared_ptr<zmq::socket_t>> m_bind;
      
      
      bool m_fragile;
      std::shared_ptr<zmq::socket_t> m_sock;
      int m_type;
      
      void _snd_strptr(const std::shared_ptr<std::string> s);
      static void strp_free(void*, void*) {return;}
      
    public:
      Socket(const int type): m_fragile(false), m_sock(nullptr), m_type(type) {}
      ~Socket() {m_sock=nullptr;}
      void connect(const char* endpt, const bool persist = true);
      void connect(const std::string& endpt, const bool persist = true){connect(endpt.c_str(), persist);}
      void bind(const char* endpt, const bool persist = true);
      void bind(const std::string& endpt, const bool persist = true) {bind(endpt.c_str(), persist);}
      
      template <class T>
      bool send(BaseMessage<T> & msg, const int opts = 0);
      
      zmq::socket_t& raw_sock();
  };
  
  template <class T>
  bool Socket::send(BaseMessage<T> & msg, const int opts)
  {
    int count = 1;
    bool win = true;
    
    static zmq::message_t z_msg; //So we don't reinitialize every time, that's just silly
    std::list<std::shared_ptr<std::string>> frames = msg.prep_frames();
    for (auto s: frames)
    {
      if (count < frames.size())
      {
	z_msg.rebuild((void*)s->c_str(), s->size(), strp_free);
	win &= raw_sock().send(z_msg, opts & ZMQ_SNDMORE);
	count ++;
      }
    }
    //now send the last frame without enforcing the SNDMORE flag
    z_msg.rebuild((void*)frames.back()->c_str(), frames.back()->size(), strp_free);
    win &= raw_sock().send(z_msg, opts);
    msg.unprep_frames();
    return win;
  }
}

#include "socket.hpp"
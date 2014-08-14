/*!
 * \file socket.h
 * \author Nathan Eloe
 * \brief Definition of a quasi-singleton socket class
 */

#pragma once

#include "context.h"
#include <exception>
#include <map>
#include <memory>
#include <string>
#include <zmq.hpp>

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
    public:
      Socket(const int type): m_fragile(false), m_sock(nullptr), m_type(type) {}
      void connect(const char* endpt, const bool persist = true);
      void connect(const std::string& endpt, const bool persist = true){connect(endpt.c_str(), persist);}
      void bind(const char* endpt, const bool persist = true);
      void bind(const std::string& endpt, const bool persist = true) {bind(endpt.c_str(), persist);}
      
      zmq::socket_t& raw_sock();
  };
}
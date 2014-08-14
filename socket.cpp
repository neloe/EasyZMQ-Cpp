/*!
 * \file socket.cpp
 * \author Nathan Eloe
 * \brief Implementation of the quasi-singleton socket class
 */

#include "socket.h"

namespace zmqcpp
{
  std::map<std::string, std::shared_ptr<zmq::socket_t>> Socket::m_conn();
  std::map<std::string, std::shared_ptr<zmq::socket_t>> Socket::m_bind();
  
  void Socket::connect(const char* endpt, const bool persist)
  {
    if (!persist || m_conn.count(std::string(endpt) == 0)
    {
      m_sock = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
      m_sock->connect(endpt);
      if (persist)
	m_conn[std::string(endpt)] = m_sock;
    }
    else
      m_sock = m_conn[std::string(endpt)];
  }
  
  void Socket::bind(const char* endpt, const bool persist)
  {
    if (!persist || m_bind.count(std::string(endpt) == 0)
    {
      m_sock = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
      m_sock->bind(endpt);
      if (persist)
	m_bind[std::string(endpt)] = m_sock;
    }
    else
      m_sock = m_bind[std::string(endpt)];
  }
  
  zmq::socket_t& Socket::raw_sock()
  {
    if (!m_conn)
      throw no_endpt;
    return *m_conn;
  }
}
/*!
 * \file socket.cpp
 * \author Nathan Eloe
 * \brief Implementation of the quasi-singleton socket class
 */

#include "socket.h"
#include "context.h"
#include "messages/message.h"

namespace zmqcpp
{
  std::shared_ptr<zmq::context_t> Context::m_ctx = nullptr;
  thread_local std::map<std::string, std::shared_ptr<zmq::socket_t>> Socket::m_conn;
  std::map<std::string, std::shared_ptr<zmq::socket_t>> Socket::m_bind;
  
  void Socket::connect(const char* endpt, const bool persist)
  {
    std::string ep(endpt);
    if (!persist)
    {
      m_sock = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
      m_sock->connect(endpt);
    }
    else
    {
      if (m_conn.count(ep) == 0)
      {
	m_conn[ep] = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
	//m_conn[ep] = std::shared_ptr<zmq::socket_t>(new zmq::socket_t(Context::get(), m_type), [] (zmq::socket_t* p) {p -> close(); delete p;});
	m_conn[ep]->connect(endpt);
      }
      m_sock = m_conn[ep];
    }
  }
  
  void Socket::bind(const char* endpt, const bool persist)
  {
    std::string ep(endpt);
    if (!persist)
    {
      m_sock = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
      m_sock->bind(endpt);
    }
    else
    {
      if (m_bind.count(ep) == 0)
      {
	m_bind[ep] = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
	m_bind[ep]->bind(endpt);
      }
      m_sock = m_bind[ep];
    }
  }
  
  zmq::socket_t& Socket::raw_sock()
  {
    if (!m_sock)
      throw no_endpt();
    return *m_sock;
  }
 
 /*
  template<>
  Socket& operator << <std::string>(Socket & sock, const std::string& data)
  {
    Message m;
    m.add_frame(data);
    sock << m;
    return sock;
  }
  
  template<>
  Socket& operator >> <std::string>(Socket & sock, std::string& data)
  {
    Message m;
    sock >> m;
    data = *(m.frames().back());
    return sock;
  }
*/
}
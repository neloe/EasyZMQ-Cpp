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
  std::map<void*, std::shared_ptr<std::string>> Socket::m_unsent;
  std::map<std::string, std::map<int, sockopt>> Socket::m_optcache;
  
  void Socket::connect(const char* endpt, const bool persist)
  {
    std::string ep(endpt);
    curr_endpt = ep;
    m_optcache[endpt] = m_sockopts;
    if (!persist)
    {
      m_sock = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
      for (auto opt: m_sockopts)
      {
	m_sock -> setsockopt(opt.first, opt.second.val.get(), opt.second.vsize);
      }
      m_sock->connect(endpt);
    }
    else
    {
      if (m_conn.count(ep) == 0)
      {
	m_conn[ep] = std::make_shared<zmq::socket_t>(zmq::socket_t(Context::get(), m_type));
	m_conn[ep]->connect(endpt);
      }
      m_sock = m_conn[ep];
    }
  }
  
  void Socket::bind(const char* endpt, const bool persist)
  {
    std::string ep(endpt);
    curr_endpt = ep;
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
}
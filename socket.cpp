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
#include <functional>
#include <iostream>

namespace zmqcpp
{
    std::shared_ptr<zmq::context_t> Context::m_ctx = nullptr;
    thread_local std::map<size_t, std::shared_ptr<zmq::socket_t>> Socket::m_conn;
    std::map<size_t, std::shared_ptr<zmq::socket_t>> Socket::m_bind;
    std::map<void *, std::shared_ptr<std::string>> Socket::m_unsent;
    std::map<std::string, std::map<int, sockopt>> Socket::m_optcache;

    void Socket::_conn()
    {
        static bool run = true;
        if (run)
        {
            m_conn_hash = hash_list (m_conn_endpts);
            if (!m_conn.count (m_conn_hash))
            {
                m_conn[m_conn_hash] = std::make_shared<zmq::socket_t> (zmq::socket_t (Context::get(), m_type));
                for (auto opt : m_sockopts)
                    m_conn[m_conn_hash] -> setsockopt (opt.first, opt.second.val.get(), opt.second.vsize);
                for (const std::string &e : m_conn_endpts)
                    m_conn[m_conn_hash]->connect (e.c_str());
            }
            m_sock = m_conn[m_conn_hash];
        }
        return;
    }
    void Socket::_bind()
    {
        static bool run = true;
        if (run)
        {
            m_bind_hash = hash_list (m_bind_endpts);
            if (!m_bind.count (m_bind_hash))
            {
                m_bind[m_bind_hash] = std::make_shared<zmq::socket_t> (zmq::socket_t (Context::get(), m_type));
                for (auto opt : m_sockopts)
                    m_bind[m_bind_hash] -> setsockopt (opt.first, opt.second.val.get(), opt.second.vsize);
                for (const std::string &e : m_bind_endpts)
                    m_bind[m_bind_hash]->bind (e.c_str());
            }
            m_sock = m_bind[m_bind_hash];
        }
        return;
    }

    size_t Socket::hash_list (std::vector<std::string> &strvec)
    {
        std::sort (strvec.begin(), strvec.end());
        std::stringstream ss;
        for (const std::string &s : strvec)
            ss << s;
        return std::hash<std::string>() (ss.str());
    }

    zmq::socket_t &Socket::raw_sock()
    {
        if (!m_sock)
            throw no_endpt();
        return *m_sock;
    }
}
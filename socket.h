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
 * \file socket.h
 * \author Nathan Eloe
 * \brief Definition of a quasi-singleton socket class
 */

#pragma once

#include <exception>
#include <map>
#include <memory>
#include <sstream>
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
  
  struct sockopt
  {
    std::shared_ptr<void> val;
    size_t vsize;
  };
  
  class Socket
  {
    private:
      // connected sockets are thread_local, but static by connection string
      static thread_local std::map<size_t, std::shared_ptr<zmq::socket_t>> m_conn;
      // bound sockets are static (not thread_local)
      static std::map<size_t, std::shared_ptr<zmq::socket_t>> m_bind;
      static std::map<std::string, std::map<int, sockopt>> m_optcache;
      // local socket ops (for before connect)
      std::map<int, sockopt> m_sockopts;
      
      
      //vector of endpoints: to enable multi-endpoint connections and binding
      std::vector<std::string> m_conn_endpts;
      std::vector<std::string> m_bind_endpts;
      size_t m_conn_hash, m_bind_hash;
      // A shared pointer to the socket
      std::shared_ptr<zmq::socket_t> m_sock;
      // Type of the socket
      int m_type;
      bool m_connd, m_bound;
      std::string curr_endpt;
      
      static std::map<void*, std::shared_ptr<std::string>> m_unsent;
      
      /*!
       * \brief the "zero-copy" deleter; the smart pointers will deal with that for us
       * \pre None
       * \post None
       */
      static void strp_free(void* ptr, void* _) {m_unsent.erase(ptr);}
    
    public:     
      size_t hash_list(std::vector<std::string> & strvec);
      
      /*!
       * \brief connects socket to all endpoints in the connection list
       * \pre None
       * \post A connection is established to all endpoitns in the connection list (if it hasn't been called before for this socket)
       */
      void _conn();
      
      /*!
       * \brief binds socket to all endpoints in the connection list
       * \pre None
       * \post A binding is established to all endpoitns in the connection list
       */
      void _bind();

      /*!
       * \brief Constructor
       * \pre None
       * \post The socket is constructed with the specified socket type
       */
      Socket(const int type): m_sock(nullptr), m_type(type) {}
      /*!
       * \brief Destructor
       * \pre None
       * \post releases the socket
       */
      ~Socket() {m_sock=nullptr;}
      ///@{
      /*!
       * \brief adds an endpoint to the connection list
       * \pre None
       * \post pushes endpt to the back of the connection list
       * \post sets the current_endpt to be the specified string
       * \throws zmq::error_t as specified by the ZMQ C++ api
       * 
       * It is highly recommended you NEVER use a fragile conenction (persist = false).  Do so at your own risk
       * I take no responsibility if fragile connections cause you to be severely irradiated
       */
      void connect(const std::string & endpt) {m_conn_endpts.push_back(endpt); curr_endpt = endpt;}
      void connect(const char* endpt) {connect(std::string(endpt));}
      
      //void connect(const char* endpt, const bool persist = true);
      //void connect(const std::string& endpt, const bool persist = true){connect(endpt.c_str(), persist);}
      ///@}
      ///@{
      /*!
       * \brief adds an endpoint to the connection list
       * \pre None
       * \post pushes endpt to the back of the binding list
       * \post sets the current_endpt to be the specified string
       * \throws zmq::error_t as specified by the ZMQ C++ api
       * 
       * * It is highly recommended you NEVER use a fragile conenction (persist = false).  Do so at your own risk
       */
      void bind(const std::string & endpt) {m_bind_endpts.push_back(endpt); curr_endpt = endpt;}
      void bind(const char* endpt) {bind(std::string(endpt));}
      //void bind(const char* endpt, const bool persist = true);
      //void bind(const std::string& endpt, const bool persist = true) {bind(endpt.c_str(), persist);}
      ///@}
      
      /*!
       * \brief sends the message over the socket with the specified options
       * \pre The type of this socket must be allowed to send (e.g. no ZMQ_PULL)
       * \pre type T has the prep_frames() and unprep_frames() functions implemented
       * \post the message (possibly multi-part) is sent over the wire
       * \post for multi-part messages, ZMQ_SNDMORE is forced for all but the last frame
       * \returns Whether the send was successful or not (as per the ZMQ C++ api)
       */
      template <class T>
      bool send(const BaseMessage<T> & msg, const int opts = 0);
      
      /*!
       * \brief recv's the message over the socket with the specified options
       * \pre The type of this socket must be allowed to recv (e.g. no ZMQ_PUSH)
       * \pre type T has the start_recv() and recv_more() functions implemented
       * \post Message frames are recv'd from the socket until recv_more() returns false
       * \returns Whether the send was successful or not (as per the ZMQ C++ api)
       */
      template <class T>
      bool recv(BaseMessage<T> & msg, const int opts = 0);
      
      /*!
       * \brief returns the raw socket
       * \pre None
       * \post None
       * \returns Reference to the zmq socket
       * 
       * ONLY call this if you absolutely need to; it is only there for advanced use cases
       * I take no responsibility if use of this function kills your goldfish
       */
      zmq::socket_t& raw_sock();
      
      ///@{
      /*!
       * \brief A templated send operator
       * \pre if T != std::string, type T must have the << operator overloaded (into a stringstream)
       * \pre the string representation from stringstream << T must be the representation to send
       * \pre the socket type must allow send
       * \post Sends a single frame with the text over the socket
       * \throws zmq::error_t
       * \returns the socket
       * 
       * \note This function hides the return value of socket.send() (for now)
       * 
       * Specialized for std::string
       */
      template <class T> friend Socket& operator << (Socket & sock, const BaseMessage<T>& data);
      //template <class T> friend Socket& operator << (Socket & sock, const T& data);
      ///@}
      ///@{
      /*!
       * \brief A templated recv operator
       * \pre if T != std::string, type T must have the >> operator overloaded (into a stringstream)
       * \pre the string representation from stringstream >> T must be the representation to receive
       * \pre the socket type must allow recv
       * \post Sends a single frame with the text over the socket
       * \throws zmq::error_t
       * \returns the socket
       * 
       * \note This function has the same downfalls as the normal >> operator; if types don't match, behavior will be unexpected
       * You should always know the type of data you are pulling in
       * 
       * \note This function hides the return value of socket.recv() (for now)
       * 
       * Specialized for std::string
       */
      template <class T> friend Socket& operator >> (Socket & sock, BaseMessage<T>& data);
      //template <class T> friend Socket& operator >> (Socket & sock, const T& data);
      ///@}
      
      /*!
       * \brief returns the endpoint this socket is currently connected to (helpful in debugging)
       * \pre None
       * \post None
       * \returns the string denoting the endpoint
       */
      const std::string & endpt() const {return curr_endpt;}
      /*!
       * \brief forces a full disconnect of this socket (next call to connect will create a new socket
       * \pre None
       * \post the socket is removed from the cache and will be created on next connect()
       */
      void disconnect() {m_conn.erase(m_conn_hash);}
      /* socket options */
      /*!
       * \brief sets a non-string sockopt (before connection)
       * \pre name and data are allowable values from the ZMQ API Spec
       * \post the sockopt is set to be set on next connect
       */
      template <class T>
      void setsockopt (const int name, const T& data)
      {
	m_sockopts[name] = {std::static_pointer_cast<void>(std::make_shared<T>(data)), sizeof(T)};
      }
      
      
  };
  
  template <class T>
  bool Socket::send(const BaseMessage<T> & msg, const int opts)
  {
    if (m_conn_endpts.size()) _conn();
    else _bind();
    int count = 1;
    bool win = true;
    
    static zmq::message_t z_msg; //So we don't reinitialize every time, that's just silly
    std::list<std::shared_ptr<std::string>> frames = msg.prep_frames();
    for (auto s: frames)
    {
      if (count < frames.size())
      {
	z_msg.rebuild((void*)s->c_str(), s->size(), strp_free);
	m_unsent[z_msg.data()] = s;
	win &= raw_sock().send(z_msg, opts | ZMQ_SNDMORE);
	count ++;
      }
    }
    //now send the last frame without enforcing the SNDMORE flag
    z_msg.rebuild((void*)frames.back()->c_str(), frames.back()->size(), strp_free);
    m_unsent[z_msg.data()] = frames.back();
    win &= raw_sock().send(z_msg, opts);
    msg.unprep_frames();
    return win;
  }
  
  template <class T>
  Socket & operator << (Socket & sock, const BaseMessage<T>& data)
  {
    sock.send(data);
    return sock;
  }
  
  template <class T>
  bool Socket::recv(BaseMessage<T> & msg, const int opts)
  {
    if (m_conn_endpts.size()) _conn();
    else _bind();
    bool win = true;
    static zmq::message_t z_msg;
    msg.start_recv();
    int more;
    size_t msize = sizeof(more);
    do
    {
      z_msg.rebuild();
      win &= raw_sock().recv(&z_msg, opts);
      msg.add_frame((char*)z_msg.data(), z_msg.size());
      raw_sock().getsockopt(ZMQ_RCVMORE, &more, &msize);
    } while (more);
    return win;
  }

  template <class T>
  Socket & operator >> (Socket & sock, BaseMessage<T>& data)
  {
    sock.recv(data);
    return sock;
  }
}

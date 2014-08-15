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
      
      //Whether or not this is fragile.  Hopefully, always false
      bool m_fragile;
      // A shared pointer to the socket
      std::shared_ptr<zmq::socket_t> m_sock;
      // Type of the socket
      int m_type;
      
      /*!
       * \brief sends the data in the string pointed to using zero copy methods
       * \pre None
       * \post The string pointed to is sent over the socket
       */
      void _snd_strptr(const std::shared_ptr<std::string> s);
      
      /*!
       * \brief the "zero-copy" deleter; the smart pointers will deal with that for us
       * \pre None
       * \post None
       */
      static void strp_free(void*, void*) {return;}
      
    public:
      /*!
       * \brief Constructor
       * \pre None
       * \post The socket is constructed with the specified socket type
       */
      Socket(const int type): m_fragile(false), m_sock(nullptr), m_type(type) {}
      /*!
       * \brief Destructor
       * \pre None
       * \post releases the socket
       */
      ~Socket() {m_sock=nullptr;}
      ///@{
      /*!
       * \brief connects the socket to the endpoint
       * \pre None
       * \post Connects a socket to the endpoint if needed (!persist and no connection to endpt previously)
       * \throws zmq::error_t as specified by the ZMQ C++ api
       * 
       * It is highly recommended you NEVER use a fragile conenction (persist = false).  Do so at your own risk
       * I take no responsibility if fragile connections cause you to be severely irradiated
       */
      void connect(const char* endpt, const bool persist = true);
      void connect(const std::string& endpt, const bool persist = true){connect(endpt.c_str(), persist);}
      ///@}
      ///@{
      /*!
       * \brief binds the socket to the endpoint
       * \pre None
       * \post Binds a socket to the endpoint if needed (!persist and no connection to endpt previously)
       * \throws zmq::error_t as specified by the ZMQ C++ api
       * 
       * * It is highly recommended you NEVER use a fragile conenction (persist = false).  Do so at your own risk
       */
      void bind(const char* endpt, const bool persist = true);
      void bind(const std::string& endpt, const bool persist = true) {bind(endpt.c_str(), persist);}
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
      bool send(BaseMessage<T> & msg, const int opts = 0);
      
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
  
  template <class T>
  bool Socket::recv(BaseMessage<T> & msg, const int opts)
  {
    bool win = true;
    static zmq::message_t z_msg;
    msg.start_recv();
    do
    {
      z_msg.rebuild();
      win &= raw_sock().recv(&z_msg, opts);
      msg.add_frame((char*)z_msg.data(), z_msg.size());
    } while (msg.recv_more());
  }
}

/*!
 * \file context.cpp
 * \author Nathan Eloe
 * \brief Implementation of the singleton context class
 */

#include "context.h"
#include "socket.h"

namespace zmqcpp
{  
  Context::Context(const unsigned int numthreads)
  {
    init(numthreads);
  }
  
  zmq::context_t& Context::init(const unsigned int numthreads)
  {
    if (!m_ctx)
      m_ctx = std::shared_ptr<zmq::context_t>(new zmq::context_t(numthreads));//, _del);
    return *m_ctx;
  }
  
  zmq::context_t& Context::get()
  {
    return init();
  }
}
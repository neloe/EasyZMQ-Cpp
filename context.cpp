/*!
 * \file context.cpp
 * \author Nathan Eloe
 * \brief Implementation of the singleton context class
 */

#include "context.h"

namespace zmqcpp
{
  std::shared_ptr<zmq::context_t> Context::m_ctx = nullptr;
  
  Context::Context(const unsigned int numthreads)
  {
    init(numthreads);
  }
  
  zmq::context_t& Context::init(const unsigned int numthreads)
  {
    if (!m_ctx)
      m_ctx = std::make_shared<zmq::context_t>(zmq::context_t(numthreads));
    return *m_ctx;
  }
  
  static zmq::context_t& Context::get()
  {
    return init();
  }
}
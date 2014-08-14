/*!
 * \file context.h
 * \author Nathan Eloe
 * \brief A singleton context class
 */

#pragma once

#include <zmq.hpp>
#include <memory>

namespace zmqcpp
{
  class Context
  {
    private:
      // Singleton member variable, to vastly simplify how much gets passed around
      static std::shared_ptr<zmq::context_t> m_ctx;
      static zmq::context_t* init(const unsigned int numthreads = 1);
    public:
      Context(const unsigned int numthreads = 1);
      static zmq::context_t& get();
  };
}
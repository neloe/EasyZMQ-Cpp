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
    static zmq::context_t &init (const unsigned int numthreads = 1);
  public:
    Context (const unsigned int numthreads = 1);
    static zmq::context_t &get();
};
}
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
 * \file context.cpp
 * \author Nathan Eloe
 * \brief Implementation of the singleton context class
 */

#include "context.h"
#include "socket.h"

namespace zmqcpp
{
Context::Context (const unsigned int numthreads)
{
    init (numthreads);
}

zmq::context_t &Context::init (const unsigned int numthreads)
{
    if (!m_ctx)
        m_ctx = std::shared_ptr<zmq::context_t> (new zmq::context_t (numthreads)); //, _del);
    return *m_ctx;
}

zmq::context_t &Context::get()
{
    return init();
}
}
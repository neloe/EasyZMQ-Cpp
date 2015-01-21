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
 * \file insert.cpp
 * \author Nathan Eloe
 * \brief tests find and findOne functionality of the mongo cxx driver
 */

#include "../zmqcpp.h"
#include "gtest/gtest.h"
#include <string>
#include <zmq.hpp>

const char BIND[] = "tcp://*:5558";
const char CONN[] = "tcp://localhost:5558";

TEST (HelperTest, SendRecv)
{
    const int VAL = 5;
    const std::string STRVAL = "5";
    zmqcpp::Socket send (ZMQ_PUSH);
    zmqcpp::Socket recv (ZMQ_PULL);
    zmqcpp::Message m;
    send.bind (BIND);
    recv.connect (CONN);
    recv._conn();
    send << zmqcpp::Message (VAL);
    recv >> m;
    ASSERT_EQ (STRVAL, * (m.frames().back()));
}


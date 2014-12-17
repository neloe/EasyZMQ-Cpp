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

const char BIND[] = "tcp://*:5557";
const char CONN[] = "tcp://localhost:5557";

TEST(MessageTest, Send)
{
  zmqcpp::Socket send(ZMQ_PUSH);
  zmqcpp::Socket recv(ZMQ_PULL);
  
  send.bind(BIND);
  recv.connect(CONN);
  recv._conn();
  const std::string DATA = "Hi world!";
  zmqcpp::Message mesg;
  mesg.add_frame(DATA);
  std::string rep;
  
  ASSERT_TRUE(send.send(mesg));
  
  zmq::message_t req;
  recv.raw_sock().recv(&req);
  rep = std::string((char*)req.data(), req.size());
  
  ASSERT_EQ(DATA, rep);  
}

TEST(MessageTest, Recv)
{
  zmqcpp::Socket send(ZMQ_PUSH);
  zmqcpp::Socket recv(ZMQ_PULL);
  
  send.bind(BIND);
  recv.connect(CONN);
  
  const std::string DATA = "Hi world!";
  zmqcpp::Message mesg;
  mesg.add_frame(DATA);
  std::string rep;
  
  ASSERT_TRUE(send.send(mesg));
  mesg.clear();
  ASSERT_EQ(0, mesg.frames().size());
  ASSERT_TRUE(recv.recv(mesg));
  ASSERT_EQ(DATA, mesg.last());  
}

TEST(MessageTest, MultiPart)
{
  zmqcpp::Socket send(ZMQ_PUSH);
  zmqcpp::Socket recv(ZMQ_PULL);
  
  send.bind(BIND);
  recv.connect(CONN);
  
  const std::string DATA = "Hi world!";
  const std::string DATA2 = "hello again";
  zmqcpp::Message mesg, recvd;
  mesg.add_frame(DATA);
  mesg.add_frame(DATA2);
  std::string rep;
  
  ASSERT_TRUE(send.send(mesg));
  ASSERT_TRUE(recv.recv(recvd));
  ASSERT_EQ(DATA, *(recvd.frames().front()));
  ASSERT_EQ(DATA2, recvd.last());
}


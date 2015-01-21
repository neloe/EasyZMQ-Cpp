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

const char BIND[] = "tcp://*:5555";
const char CONN[] = "tcp://localhost:5555";
const char BIND2[] = "tcp://*:5556";
const char CONN2[] = "tcp://localhost:5556";
const char BIND3[] = "tcp://*:5559";
const char CONN3[] = "tcp://localhost:5559";

/*TEST(SocketTest, CreateFragileAndSend)
{
  //Try creating the context here
  zmqcpp::Context c;
  zmqcpp::Socket send(ZMQ_REQ);
  zmqcpp::Socket recv(ZMQ_REP);

  send.bind(BIND2);
  recv.connect(CONN2);

  const std::string DATA = "Hi world!";
  std::string rep;

  zmq::message_t msg(DATA.size());
  memcpy((void*) msg.data(), DATA.c_str(), DATA.size());
  send.raw_sock().send(msg);

  zmq::message_t req;
  recv.raw_sock().recv(&req);
  rep = std::string((char*)req.data(), req.size());

  ASSERT_EQ(DATA, rep);

  req.rebuild(static_cast<int>(rep.size()));
  memcpy((void*)req.data(), rep.c_str(), rep.size());
  recv.raw_sock().send(req);

  zmq::message_t final;
  send.raw_sock().recv(&final);
  std::string finstr((char*)final.data(), final.size());

  ASSERT_EQ(DATA, finstr);

}*/

TEST (SocketTest, CreateAndSend)
{
    zmqcpp::Socket send (ZMQ_REQ);
    zmqcpp::Socket recv (ZMQ_REP);
    send.bind (BIND);
    recv.connect (CONN);
    send._bind();
    recv._conn();
    const std::string DATA = "Hi world!";
    std::string rep;
    zmq::message_t msg (DATA.size());
    memcpy ((void *) msg.data(), DATA.c_str(), DATA.size());
    send.raw_sock().send (msg);
    zmq::message_t req;
    recv.raw_sock().recv (&req);
    rep = std::string ((char *)req.data(), req.size());
    ASSERT_EQ (DATA, rep);
    req.rebuild (static_cast<int> (rep.size()));
    memcpy ((void *)req.data(), rep.c_str(), rep.size());
    recv.raw_sock().send (req);
    zmq::message_t final;
    send.raw_sock().recv (&final);
    std::string finstr ((char *)final.data(), final.size());
    ASSERT_EQ (DATA, finstr);
}

TEST (SocketTest, NoEndPtThrow)
{
    zmqcpp::Socket a (ZMQ_STREAM);
    ASSERT_THROW (a.raw_sock(), zmqcpp::no_endpt);
    try
    {
        a.raw_sock();
        FAIL();
    }
    catch (zmqcpp::no_endpt &e)
    {
        ASSERT_STREQ ("Socket has no endpoint", e.what());
    }
}

TEST (SocketTest, MultiConn)
{
    zmqcpp::Socket b1 (ZMQ_REQ), b2 (ZMQ_REQ);
    zmqcpp::Socket s1 (ZMQ_REP);
    char data[10];
    b1.bind (BIND2);
    b1._bind();
    b2.bind (BIND3);
    b2._bind();
    s1.connect (CONN2);
    s1.connect (CONN3);
    s1._conn();
    b1.raw_sock().send ("Hi1", 3, 0);
    b2.raw_sock().send ("Hi2", 3, 0);
    s1.raw_sock().recv (data, 10, 0);
    ASSERT_STREQ ("Hi1", data);
    s1.raw_sock().send ("W1", 2, 0);
    s1.raw_sock().recv (data, 10, 0);
    ASSERT_STREQ ("Hi2", data);
    s1.raw_sock().send ("W2", 2, 0);
    for (int i = 0; i < 10; i++)
        data[i] = 0;
    b1.raw_sock().recv (data, 10, 0);
    ASSERT_STREQ ("W1", data);
    for (int i = 0; i < 10; i++)
        data[i] = 0;
    b2.raw_sock().recv (data, 10, 0);
    ASSERT_STREQ ("W2", data);
}


/*!
 * \file insert.cpp
 * \author Nathan Eloe
 * \brief tests find and findOne functionality of the mongo cxx driver
 */

#include "../socket.h"
#include "../context.h"
#include "../messages/message.h"
#include "gtest/gtest.h"
#include <string>
#include <zmq.hpp>

const char BIND[] = "tcp://*:5556";
const char CONN[] = "tcp://localhost:5556";

TEST(HelperTest, SendRecv)
{
  const int VAL = 5;
  const std::string STRVAL = "5";
  zmqcpp::Socket send(ZMQ_PUSH);
  zmqcpp::Socket recv(ZMQ_PULL);
  
  zmqcpp::Message m;
  
  send.bind(BIND);
  recv.connect(CONN);
  
  zmqcpp::Message m2(VAL);
  send << m2;
  recv >> m;
  ASSERT_EQ(STRVAL, *(m.frames().back()));
}


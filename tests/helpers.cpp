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

TEST(HelperTest, SendRecv)
{
  const int VAL = 5;
  const std::string STRVAL = "5";
  zmqcpp::Socket send(ZMQ_PUSH);
  zmqcpp::Socket recv(ZMQ_PULL);
  
  zmqcpp::Message m;
  
  send.bind(BIND);
  recv.connect(CONN);
  
  send << zmqcpp::Message(VAL);
  recv >> m;
  ASSERT_EQ(STRVAL, *(m.frames().back()));
}


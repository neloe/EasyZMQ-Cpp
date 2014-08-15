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

const char BIND[] = "tcp://*:5557";
const char CONN[] = "tcp://localhost:5557";

TEST(MessageTest, Send)
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
  zmqcpp::Message mesg, recvd;
  mesg.add_frame(DATA);
  std::string rep;
  
  ASSERT_TRUE(send.send(mesg));
  ASSERT_TRUE(recv.recv(recvd));
  ASSERT_EQ(DATA, *(recvd.frames().front()));  
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
  ASSERT_EQ(DATA, *(recvd.frames().back()));
  ASSERT_TRUE(recv.recv(recvd));
  ASSERT_EQ(DATA2, *(recvd.frames().back()));
}


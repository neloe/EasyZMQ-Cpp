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

TEST(MessageTest, CreateAndSend)
{
  zmqcpp::Socket send(ZMQ_REQ);
  zmqcpp::Socket recv(ZMQ_REP);
  
  send.bind(BIND);
  recv.connect(CONN);
  
  const std::string DATA = "Hi world!";
  zmqcpp::Message mesg;
  mesg.add_frame(DATA);
  std::string rep;
  
  send.send(mesg);
  
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
  
}

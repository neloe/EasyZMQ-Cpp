/*!
 * \file insert.cpp
 * \author Nathan Eloe
 * \brief tests find and findOne functionality of the mongo cxx driver
 */

#include "../socket.h"
#include "../context.h"
#include "gtest/gtest.h"
#include <string>
#include <zmq.hpp>

const char BIND[] = "tcp://*:5555";
const char CONN[] = "tcp://localhost:5555";

TEST(SocketTest, CreateFragileAndSend)
{
  //Try creating the context here
  zmqcpp::Context c;
  zmqcpp::Socket send(ZMQ_REQ);
  zmqcpp::Socket recv(ZMQ_REP);
  
  send.bind(BIND, false);
  recv.connect(CONN, false);
  
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
  
}

TEST(SocketTest, CreateAndSend)
{
  zmqcpp::Socket send(ZMQ_REQ);
  zmqcpp::Socket recv(ZMQ_REP);
  
  send.bind(BIND);
  recv.connect(CONN);
  
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
  
}

TEST(SocketTest, NoEndPtThrow)
{
  zmqcpp::Socket a(ZMQ_STREAM);
  ASSERT_THROW(a.raw_sock(), zmqcpp::no_endpt);
  try
  {
    a.raw_sock();
    FAIL();
  }
  catch (zmqcpp::no_endpt & e)
  {
    ASSERT_STREQ("Socket has no endpoint", e.what());
  }
}

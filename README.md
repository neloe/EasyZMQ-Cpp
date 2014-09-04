# High Level C++ ZMQ API

## Prerequisites
The following are required to build the ZMQCPP api:
* CMake
* The ZMQ libraries (ubuntu/debian package libzmq3-dev
* A modern C++ compiler supporting C++11 (clang++ 3.x, g++ 4.7+)

## Build Instructions

This software uses CMake as it's build system; as such out of source builds are preferred.

You should create a build directory in the source directory and cd to it.

To prepare for release (from the build directory) :
`cmake .. && make zmqcpp`

To run code coverage:
`cmake .. -DCMAKE_BUILD_TYPE=Debug && make zmqcpp_coverage`

To generate Cobertura xml output for code coverage:
`cmake .. -DCMAKE_BUILD_TYPE=Debug && make zmqcpp_cobertura`

## Getting Started

### Creating sockets
Creating a socket is as easy as `zmqcpp::Socket sock_name(ZMQ_SOCKTYPE)`.  Examples
```c++
zmqcpp::Socket send(ZMQ_PUSH);
zmqcpp::Socket recv(ZMQ_PULL);

//bind and connect using the appropriate function
sendsock.bind(BIND_ADDR);
recvsock.connect(CONN_ADDR);
```
The need for a Context is hidden in a singleton class; if you absolutely need to get the automatically created context:
```c++
zmqcpp::Context c;
c.get();
// or
zmqcpp::Context::get();
```
### Using sockets
All communication through sockets must happen through the `zmqcpp::BaseMessage<T>` interface.  One basic message type is defined: `zmqcpp::Message`.  Messages can be created using the templated constructor; if the << operator is overloaded to allow insertion into a stringstream, and the resulting string is the correct format, the Message constructor will work just fine.
```c++
zmqcpp::Message m1(1);
zmqcpp::Message m2("hello");
```
More complex (read: multi-frame) messages can be created using the `add_frame` method
```c++
zmqcpp::Message m3;
m3.add_frame("hello");
m3.add_frame("world");
```
Messages can then be sent/received one of two ways:
```c++
zmqcpp::Message rec;
std::string last_recvd;
sendsock.send(m1);
recvsock.recv(rec);
last_recvd = *(rec.frames().back()); // contains the last frame received, last_recvd == "1"
//can also use:
last_recvd = rec.last();
```
Or
```c++
sendsock << m3;
recvsock >> rec;
cout << *(rec.frames().back()) << endl; // prints "hello" to the screen
revsock >> rec; // rec's frame list now contains "1", "hello", "world"
cout << rec.last() << endl; // prints "world" to the screen, does the same as above
```
This now works: `sendsock << zmqcpp::Message(4)`. 
More complex message types can be created by inheriting from the base message type; these messages can handle special protocol messages, etc.
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
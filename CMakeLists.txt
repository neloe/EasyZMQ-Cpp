project(zmqcpp)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -Wno-deprecated-register ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -Wno-deprecated-register -O0 --coverage ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/modules/)

set( zmqsrc
     socket.cpp
     context.cpp
)

add_library (zmqcpp SHARED ${zmqsrc})
target_link_libraries(zmqcpp zmq)

add_subdirectory(tests)

if (CMAKE_BUILD_TYPE MATCHES Debug)
include(CodeCoverage)
setup_target_for_coverage(${PROJECT_NAME}_coverage zmqtests coverage)
setup_target_for_coverage_cobertura(${PROJECT_NAME}_cobertura zmqtests coverage --gtest_output=xml:coverage.junit.xml)
endif()
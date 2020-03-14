# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

project(send-anything)

set(EXEC send-anything-server)

set(CMAKE_CXX_STANDARD 14)

set (CMAKE_CXX_FLAGS "-lboost_system -lboost_thread -pthread")

include_directories(includes /usr/local/boost_1_69_0)

file(GLOB SOURCES *.cpp)

add_executable(${EXEC} ${SOURCES})

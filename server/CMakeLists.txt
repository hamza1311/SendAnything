# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

project(send-anything)

set(EXEC send-anything-server)

set(CMAKE_CXX_STANDARD 14)

include_directories(includes)

file(GLOB SOURCES *.cpp)

add_executable(${EXEC} ${SOURCES})

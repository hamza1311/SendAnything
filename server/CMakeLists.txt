# only for cmake --version >= 3.5.1
cmake_minimum_required(VERSION 3.5.1)

project(send-anything)

FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtkmm-3.0)

set(EXEC send-anything-server)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-lboost_system -lboost_thread -pthread ")

include_directories(includes ${GTK3_INCLUDE_DIRS})

file(GLOB SOURCES *.cpp)
file(GLOB IMPL_SOURCES impl/*.cpp)

add_executable(${EXEC} ${IMPL_SOURCES} ${SOURCES})
target_link_libraries(${EXEC} ${GTK3_LIBRARIES})
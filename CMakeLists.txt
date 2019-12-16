cmake_minimum_required(VERSION 3.01)
project(chat_project)

set(CMAKE_CXX_STANDARD 14)
set(Boost_INCLUDE_DIR /usr/local/include/boost)
set(Boost_LIBRARY_DIR /usr/local/lib)

link_libraries(/usr/local/lib/libhiredis.a)
link_libraries(/usr/local/lib/libglog.so)
include_directories(${Boost_INCLUDE_DIR})

add_executable(chat_server main.cpp Server.cpp MessageQueue.cpp MessageQueue.h Redis.cpp Redis.h MessageJson.cpp MessageJson.h event_infor.h)
add_executable(chat_test test.cpp Redis.cpp MessageJson.cpp)

target_link_libraries(chat_server pthread hiredis glog)

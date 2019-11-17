cmake_minimum_required(VERSION 3.01)
project(chat_project)

set(CMAKE_CXX_STANDARD 17)
link_libraries(/usr/local/lib/libhiredis.a)
link_libraries(/usr/local/lib/libglog.so)
add_executable(chat_server main.cpp Server.cpp MessageQueue.cpp MessageQueue.h Redis.cpp Redis.h)
add_executable(chat_Cilent Cilent.cpp)

target_link_libraries(chat_server pthread hiredis glog)
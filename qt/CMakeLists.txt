cmake_minimum_required(VERSION 3.5)

project(untitled LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(Boost_INCLUDE_DIR /usr/local/include/boost)
#set(Boost_LIBRARY_DIR /usr/local/lib)

#set(Qt5Widgets_DIR /home/daimiaopeng/Qt5.14.0/5.14.0/gcc_64/include/Qt5Widgets_DIR)
set(Qt5_DIR /home/daimiaopeng/Qt5.14.0/5.14.0/gcc_64/lib/cmake/Qt5)


find_package(Qt5 COMPONENTS Widgets REQUIRED Network)


add_executable(untitled
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        client.cpp
        client.h
        client.ui
        socket.h
        socket.cpp
        register.cpp
        register.h
        register.ui
        )

target_link_libraries(untitled PRIVATE Qt5::Widgets Qt5::Core Qt5::Network)

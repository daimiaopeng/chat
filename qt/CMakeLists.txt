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
#set(Qt5_DIR /home/daimiaopeng/Qt5.14.0/5.14.0/gcc_64/lib/cmake/Qt5)


find_package(Qt5 COMPONENTS Widgets REQUIRED Network)

if(WIN32)
	if(MSVC)
		set_target_properties(${PROJECT_NAME} PROPERTIES
			WIN32_EXECUTABLE YES
			LINK_FLAGS "/ENTRY:mainCRTStartup"
		)
	elseif(CMAKE_COMPILER_IS_GNUCXX)
			# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows") # Not tested
	else()
		message(SEND_ERROR "You are using an unsupported Windows compiler! (Not MSVC or GCC)")
	endif(MSVC)
elseif(APPLE)
	set_target_properties(${PROJECT_NAME} PROPERTIES
			MACOSX_BUNDLE YES
	)
elseif(UNIX)
	# Nothing special required
else()
	message(SEND_ERROR "You are on an unsupported platform! (Not Win32, Mac OS X or Unix)")
endif(WIN32)

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

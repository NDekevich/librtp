cmake_minimum_required(VERSION 2.8 FATAL_ERROR)


set(PROJECT_NAME Ipf.proto.tests)
project(${PROJECT_NAME})


set(CMAKE_CXX_STANDARD 11)
set(GLOBAL PROPERTY USE_FOLDERS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Boost 1.58.0 REQUIRED)
find_package(GTest REQUIRED)

 set(SRC_FILES
    src/stdafx.cpp
    src/testsRtp.cpp
    )
    
    set(HDR_FILES
    include/stdafx.h
    include/targetver.h 
    )
    
target_link_libraries (${PROJECT_NAME} ../build/Debug/Ipf.proto.lib)
add_executable(${PROJECT_NAME} ${SRC_FILES})

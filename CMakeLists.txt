cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(PROJECT_NAME Ipf.proto)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(GLOBAL PROPERTY USE_FOLDERS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)

find_package(Threads REQUIRED)
find_package(Boost 1.58.0 REQUIRED
    COMPONENTS system date_time regex program_options)

set(SRC_FILES
    src/rtp.cpp
    src/rtcp.cpp
    src/controlBlock.cpp
    )

set(HDR_FILES
    include/controlBlock.h
    include/rtp.h
    include/rtcp.h
    )

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HDR_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR} include)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/ext/gtest)
    add_subdirectory(test)
endif()
add_subdirectory(example)



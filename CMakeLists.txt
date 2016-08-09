cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(PROJECT_NAME Ipf.proto)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 11)
set(GLOBAL PROPERTY USE_FOLDERS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_LIBS ON)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(ExtProjectUtils)

find_package(Threads REQUIRED)
find_package(Boost 1.58.0 REQUIRED
    COMPONENTS system date_time regex program_options)

#ExtProjectGit("https://github.com/snikulov/opus-1.1-CMake.git" "cmake_install" "${CMAKE_BINARY_DIR}/deps/")

set(SRC_FILES
    src/rtp.cpp
    src/rtcp.cpp
    src/controlBlock.cpp
    src/opusInterface.cpp
    src/codecInterface.cpp
    )

set(HDR_FILES
    include/controlBlock.h
    include/rtp.h
    include/rtcp.h
    include/codecInterface.h
    include/opusInterface.h
    )

add_library(${PROJECT_NAME} STATIC ${SRC_FILES} ${HDR_FILES})

FIND_LIBRARY(OPUS NAMES "opus" "celt" "silk_common" "silk_fixed" "silk_float"
  PATHS ${CMAKE_BINARY_DIR}/deps/)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/deps/include include)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR} include)


include(CTest)
#add_subdirectory(${PROJECT_SOURCE_DIR}/ext/opus)
if(BUILD_TESTING)
    add_subdirectory(${PROJECT_SOURCE_DIR}/ext/gtest)
    add_subdirectory(test)
endif()
add_subdirectory(example)



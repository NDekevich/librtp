cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

set(SRC_FILES
    sender.cpp
    )
    
set(PROJECT_NAME sender_rtp)
set(DEPS_LOCATION "${CMAKE_BINARY_DIR}/deps")

link_directories(${DEPS_LOCATION}/lib)
include_directories(${DEPS_LOCATION}/include)

add_executable(${PROJECT_NAME} ${SRC_FILES})

#message(WARNING "deps: " ${DEPS_LOCATION})
#message(WARNING "opus: " ${opus})
target_link_libraries(${PROJECT_NAME} Ipf.proto ${Boost_LIBRARIES})
find_library(${opus_lib} NAMES "opus.lib" "celt.lib" "silk_common.lib"
            PATH ${DEPS_LOCATION}/deps)

message(WARNING "opus: " ${opus})


message(WARNING "celt: " ${celt})

message(WARNING "opus_lib: " ${opus_lib})



#add_dependencies(${PROJECT_NAME} ${opus})
link_libraries(${PROJECT_NAME} ${opus})
target_link_libraries(${PROJECT_NAME} ${opus})

target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${DEPS_LOCATION}/include include)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
if(WIN32)
    target_link_libraries(${PROJECT_NAME} ws2_32 advapi32 Iphlpapi Winmm)
endif()



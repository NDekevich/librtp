set(SRC_FILES
    sender.cpp
    )

set(PROJECT_NAME sender_rtp)

add_executable(${PROJECT_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_NAME} Ipf.proto ${Boost_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

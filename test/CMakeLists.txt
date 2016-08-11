set(SRC_FILES
    testsRtp.cpp
    )

set(PROJECT_TEST_NAME test_rtp)
set(DEPS_LOCATION "${CMAKE_BINARY_DIR}/deps")
link_directories(${DEPS_LOCATION}/lib)
include_directories(${DEPS_LOCATION}/include)

add_executable(${PROJECT_TEST_NAME} ${SRC_FILES})
target_link_libraries(${PROJECT_TEST_NAME} Ipf.proto ${Boost_LIBRARIES})

link_libraries(${PROJECT_TEST_NAME} ${opus})
target_link_libraries(${PROJECT_TEST_NAME} ${opus})

target_include_directories(${PROJECT_TEST_NAME} PRIVATE ${GTEST_INCLUDE_DIRS} ${Boost_INCLUDE_DIR} ${PROJECT_SOURCE_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${DEPS_LOCATION}/include include)
add_dependencies(${PROJECT_TEST_NAME} googletest)




if(NOT WIN32 OR MINGW)
    target_link_libraries(${PROJECT_TEST_NAME}
        ${GTEST_LIBS_DIR}/libgtest.a
        ${GTEST_LIBS_DIR}/libgtest_main.a
        )
else()
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
    target_link_libraries(${PROJECT_TEST_NAME}
        debug ${GTEST_LIBS_DIR}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        optimized ${GTEST_LIBS_DIR}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest_main${CMAKE_FIND_LIBRARY_SUFFIXES}
        )
endif()

if(WIN32)
    target_link_libraries(${PROJECT_TEST_NAME} ws2_32 advapi32 Iphlpapi)
endif()

target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_test(test1 ${PROJECT_TEST_NAME})


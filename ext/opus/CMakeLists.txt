cmake_minimum_required(VERSION 2.8.8)
project(opus)
include(ExternalProject)

ExternalProject_Add(opus
    GIT_REPOSITORY https://github.com/snikulov/opus-1.1-CMake.git
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -DCMAKE_CXX_FLAGS=${MSVC_COMPILER_DEFS}
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    
    
)

ExternalProject_Get_Property(opus source_dir)
set(OPUS_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)

ExternalProject_Get_Property(opus binary_dir)
set(OPUS_LIBS_DIR ${binary_dir}/opus/lib PARENT_SCOPE)

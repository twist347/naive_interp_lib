cmake_minimum_required(VERSION 3.26)

set(T test_1d_algs_accuracy)

project(${T})

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/_1d)
include_directories(${CMAKE_SOURCE_DIR}/include/utility)

add_executable(${T} ${SOURCES})

target_link_libraries(${T} gtest tbb)
cmake_minimum_required(VERSION 3.26)
project(test_1d)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_1d ${SOURCES}
        test_utils.h)

target_link_libraries(test_1d gtest)
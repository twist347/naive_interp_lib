cmake_minimum_required(VERSION 3.26)
project(test_1d_perf)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/_1d)

add_executable(test_1d_perf ${SOURCES})

target_link_libraries(test_1d_perf gtest OpenMP::OpenMP_CXX gsl)


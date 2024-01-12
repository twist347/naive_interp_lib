cmake_minimum_required(VERSION 3.26)
project(test_2d_perf_scat)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/_2d)

add_executable(test_2d_perf_scat ${SOURCES})

target_link_libraries(test_2d_perf_scat gtest OpenMP::OpenMP_CXX CGAL::CGAL gsl ${ARMADILLO_LIBRARIES})

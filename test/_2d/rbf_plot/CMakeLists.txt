cmake_minimum_required(VERSION 3.26)
project(test_2d_rbf_plot)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/_2d)

add_executable(test_2d_plot_rbf ${SOURCES})

target_link_libraries(test_2d_plot_rbf OpenMP::OpenMP_CXX CGAL::CGAL ${ARMADILLO_LIBRARIES})


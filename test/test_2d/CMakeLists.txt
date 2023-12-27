cmake_minimum_required(VERSION 3.26)
project(test_2d)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_2d ${SOURCES})

if (MSVC)
    target_compile_options(test_2d gtest /openmp CGAL::CGAL)
else ()
    target_link_libraries(test_2d gtest OpenMP::OpenMP_CXX CGAL::CGAL)
    #    target_compile_options(test_2d PRIVATE ${OpenMP_CXX_FLAGS} )
endif ()

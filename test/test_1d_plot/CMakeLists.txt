cmake_minimum_required(VERSION 3.26)
project(test_1d_plot)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_1d_plot ${SOURCES})

if (MSVC)
    target_compile_options(test_1d_plot /openmp)
else ()
    target_link_libraries(test_1d_plot OpenMP::OpenMP_CXX)
    #    target_compile_options(test_1d_plot PRIVATE ${OpenMP_CXX_FLAGS})
endif ()


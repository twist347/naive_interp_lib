cmake_minimum_required(VERSION 3.26)
project(test_1d_perf)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include)

add_executable(test_1d_perf ${SOURCES})

if (MSVC)
    target_compile_options(test_1d_perf gtest /openmp)
else ()
    target_link_libraries(test_1d_perf gtest OpenMP::OpenMP_CXX)
    #    target_compile_options(omp PRIVATE ${OpenMP_CXX_FLAGS})
endif ()


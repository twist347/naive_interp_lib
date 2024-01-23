cmake_minimum_required(VERSION 3.26)
project(test_1d_base)

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES ./main.cpp)

include_directories(${CMAKE_SOURCE_DIR}/include/_1d)

add_executable(test_1d ${SOURCES})

target_link_libraries(test_1d gtest OpenMP::OpenMP_CXX gsl)

#if (MSVC)
#    target_compile_options(test_1d gtest /openmp)
#else ()
#    target_link_libraries(test_1d gtest OpenMP::OpenMP_CXX gsl)
#    #    target_compile_options(test_1d PRIVATE ${OpenMP_CXX_FLAGS})
#endif ()

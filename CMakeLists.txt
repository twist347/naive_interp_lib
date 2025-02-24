cmake_minimum_required(VERSION 3.20)

set(T naive_interp_lib)

project(${T})

set(CMAKE_CXX_STANDARD 20)

# or set -DBUILD_TESTS=ON for build tests
option(BUILD_TESTS "Build tests" OFF)

# Intel Thread Building Block library for Execution Policies
find_package(TBB REQUIRED)

# header only library
add_library(${T} INTERFACE)
target_include_directories(${T} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${T} INTERFACE TBB::tbb)

# build test
if (BUILD_TESTS)
    #find Google Test
    find_package(GTest REQUIRED)
    #find Google Benchmark
    find_package(benchmark REQUIRED)

#enable CTest
#    enable_testing()

    # add sanitizers in Debug build for Clang|GCC
    if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        add_compile_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
        add_link_options(-fsanitize=address -fsanitize=undefined -fsanitize=leak)
    endif()

    add_subdirectory(test)
endif ()
cmake_minimum_required(VERSION 3.15)

project(naive_interp_lib)

set(CMAKE_CXX_STANDARD 20)

find_package(GTest REQUIRED)
find_package(benchmark REQUIRED)

find_package(TBB REQUIRED)

if (UNIX AND CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak -fno-sanitize-recover=all")
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(ni INTERFACE)
target_include_directories(ni INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ni INTERFACE TBB::tbb)

add_subdirectory(test/_1d/algs_accuracy_test)
add_subdirectory(test/_1d/gperf_test)
add_subdirectory(test/_1d/plot)
add_subdirectory(test/_1d/any_interp)

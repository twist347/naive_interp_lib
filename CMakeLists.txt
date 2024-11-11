cmake_minimum_required(VERSION 3.26)

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

#include_directories(${Boost_INCLUDE_DIRS})
#include_directories(${ARMADILLO_INCLUDE_DIRS})

add_library(ni INTERFACE
        include/_1d/i_1d_linear_alg.h)
target_include_directories(ni INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ni INTERFACE ${Boost_LIBRARIES} CGAL::CGAL ${ARMADILLO_LIBRARIES})

add_subdirectory(test/_1d/algs_accuracy_test)
#add_subdirectory(test/_1d/perf_test)
add_subdirectory(test/_1d/gperf_test)
add_subdirectory(test/_1d/plot)

#add_subdirectory(test/_2d/scat_plot)

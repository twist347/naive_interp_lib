cmake_minimum_required(VERSION 3.26)

project(naive_interp_lib)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenMP REQUIRED)
find_package(CGAL REQUIRED)
find_package(Boost REQUIRED)
find_package(GSL REQUIRED)
find_package(Armadillo REQUIRED)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak -fno-sanitize-recover=all")
endif ()

include_directories(include/_1d)
include_directories(include/_2d)
include_directories(include/)
include_directories(test/)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${ARMADILLO_INCLUDE_DIRS})

add_library(ni INTERFACE)
target_include_directories(ni INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ni INTERFACE OpenMP::OpenMP_CXX ${Boost_LIBRARIES} CGAL::CGAL gsl ${ARMADILLO_LIBRARIES})

add_subdirectory(test/_1d/base)
add_subdirectory(test/_1d/perf)
add_subdirectory(test/_1d/plot)

add_subdirectory(test/_2d/base)
add_subdirectory(test/_2d/perf_scat)
add_subdirectory(test/_2d/perf_rbf)
add_subdirectory(test/_2d/plot_scat)
add_subdirectory(test/_2d/plot_rbf)
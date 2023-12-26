cmake_minimum_required(VERSION 3.26)

project(naive_interp_lib)

set(CMAKE_CXX_STANDARD 23)

find_package(OpenMP REQUIRED)
find_package(CGAL REQUIRED)
find_package(Boost REQUIRED)

if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address,undefined,leak -fno-sanitize-recover=all")
endif ()

include_directories(${Boost_INCLUDE_DIRS})

add_library(ni INTERFACE)
target_include_directories(ni INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(ni INTERFACE OpenMP::OpenMP_CXX ${Boost_LIBRARIES} CGAL::CGAL)

add_subdirectory(test/test_1d)
add_subdirectory(test/test_1d_perf)
add_subdirectory(test/test_2d)
add_subdirectory(test/test_2d_perf)
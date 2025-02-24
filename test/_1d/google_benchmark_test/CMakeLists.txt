set(T test_1d_google_benchmark)

project(${T})

set(CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES ./main.cpp)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/test
)

add_executable(${T} ${SOURCES})

target_link_libraries(${T} PRIVATE tbb benchmark::benchmark)

#enable CTest
#add_test(NAME ${T} COMMAND ${T})
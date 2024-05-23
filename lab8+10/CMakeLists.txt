cmake_minimum_required(VERSION 3.22)

project(lab8 CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")
set(CMAKE_CXX_STANDARD_REQUIRED True)


# find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_subdirectory(src/analyzer)
add_subdirectory(src/calculator)
add_subdirectory(src/generator)

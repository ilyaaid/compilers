set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

project(lab9 CXX)

cmake_minimum_required(VERSION 3.1)

# find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

add_subdirectory(src)


cmake_minimum_required(VERSION 3.19)

project(rpc)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-g -O2 -Wall -Werror -Wno-unused)

include_directories(./)

file(GLOB_RECURSE SOURCES "serialize/*.cpp")

add_executable(main ${SOURCES} "main.cpp")

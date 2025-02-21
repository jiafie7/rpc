cmake_minimum_required(VERSION 3.19)

project(rpc)

set(CMAKE_CXX_STANDARD 14)

add_compile_options(-g -O2 -Wall -Werror -Wno-unused)

include_directories(./)

file(GLOB_RECURSE SOURCES "serialize/*.cpp" "frame/*.cpp" "rpc/*.cpp" "socket/*.cpp" "task/*.cpp" "thread/*.cpp" "utility/*.cpp")

add_executable(function_test ${SOURCES} "function_test.cpp")
add_executable(server ${SOURCES} server.cpp)
add_executable(client ${SOURCES} client.cpp)

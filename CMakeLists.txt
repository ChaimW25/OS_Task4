cmake_minimum_required(VERSION 3.20)
project(server C)

set(CMAKE_C_STANDARD 99)

add_executable(server server.cpp)
add_executable(client client.cpp mutex.c stack.c)
add_executable(mut mutex.c)
add_executable(stack stack.c)
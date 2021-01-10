cmake_minimum_required(VERSION 3.17)
project(lab6 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab6 main.c)
add_executable(lab61 server.c)
add_executable(lab62 client.c)
add_executable(lab6s serverExample.c)
add_executable(lab6c clientExample.c)
add_executable(test test.c)
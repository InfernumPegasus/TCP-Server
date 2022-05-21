cmake_minimum_required(VERSION 3.20)
project(server)

set(CMAKE_C_STANDARD 23)

add_executable(server server.c functions.c)
add_executable(client client.c functions.c)

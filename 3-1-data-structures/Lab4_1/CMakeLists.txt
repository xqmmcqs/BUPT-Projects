cmake_minimum_required(VERSION 3.13)
project(Lab4_1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(SOURCE_FILES main.c binarytree.c)

add_executable(main ${SOURCE_FILES})
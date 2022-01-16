cmake_minimum_required(VERSION 3.13)
project(Lab1)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.c list.c list.h)

add_executable(main ${SOURCE_FILES})
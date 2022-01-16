cmake_minimum_required(VERSION 3.17)
project(Homework_2_1)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "-D DEBUG")
add_executable(main main.cpp circle.cpp circle.h main.cpp)
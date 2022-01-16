cmake_minimum_required(VERSION 3.20)
project(Syntactic)

set(CMAKE_CXX_STANDARD 11)

add_executable(Syntactic main.cpp grammar.cpp grammar.h parser.cpp parser.h)

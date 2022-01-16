cmake_minimum_required(VERSION 3.20)
project(Lexical)

set(CMAKE_CXX_STANDARD 11)

add_executable(Lexical main.cpp token.cpp token.h scanner.cpp scanner.h lexer.cpp lexer.h dict.cpp dict.h)

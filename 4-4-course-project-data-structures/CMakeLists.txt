cmake_minimum_required(VERSION 3.17)
project(DS_Design)
set(CMAKE_CXX_STANDARD 14)

set(INC_DIR ~/vcpkg/installed/x64-osx/include)
set(LINK_DIR ~/vcpkg/installed/x64-osx/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})

add_executable(main main.cpp graph.cpp graph.h)
target_link_libraries(main jsoncpp restbed pthread)


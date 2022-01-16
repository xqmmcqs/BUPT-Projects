cmake_minimum_required(VERSION 3.17)
project(Phase2)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_PREFIX_PATH "$HOME/Qt/5.15.2/clang_64")

find_package(Qt5 COMPONENTS Sql REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(main main.cpp src/users.cpp include/users.h src/database.cpp include/database.h src/commodity.cpp include/commodity.h src/order.cpp include/order.h)
target_link_libraries(main Qt5::Core Qt5::Sql)
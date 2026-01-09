cmake_minimum_required(VERSION 3.10)
project(OnlineStore)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

add_executable(shop src/main.cpp src/menu.cpp)
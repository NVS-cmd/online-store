cmake_minimum_required(VERSION 3.10)
project(OnlineStore)

set(CMAKE_CXX_STANDARD 17)
include_directories(include)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -mconsole -static-libgcc -static-libstdc++")

add_executable(shop 
    src/main.cpp
    src/database.cpp
)

target_link_libraries(shop pqxx pq)
